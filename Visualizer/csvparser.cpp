#include "csvparser.h"

#include <QFile>
#include <QTimer>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>

CSVParser::CSVParser(QObject *parent) : QObject(parent)
  , m_model(new Model(this))
{
  /**

 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584264543426_csv.gzip.csv 10:29
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584264564996_csv.gzip.csv 10:29
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584265087397_csv.gzip.csv 10:48
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584265684973_csv.gzip.csv 11:15
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584269140002_csv.gzip.csv 12:01
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584272470382_csv.gzip.csv 12:41:10 - 13:01
 /home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584279713505_csv.gzip.csv 14:50

   */
}

void CSVParser::loadFile(const QString &path)
{
  m_sequence.clear();

  QFile file(path);
  if (!file.open(QFile::ReadOnly)) {
    qDebug() << "error";
    emit error(tr("Could not open file. '%1'").arg(file.errorString()));
    return;
  }

  QTextStream stream(&file);
  stream.setCodec("utf-8");

  bool skip_header = true;
  while (!stream.atEnd()) {
    QStringList fields = stream.readLine().split(',');
    if (skip_header) {
      skip_header = false;
      continue;
    }

    if (fields.size() != 12) {
      emit error(tr("Invalid row count '%1'").arg(fields.join(",")));
      continue;
    }

    Model::Data data;
    data.timestamp = fields.at(0).toULongLong();
    data.latitude = fields.at(1).toDouble();
    data.longitude = fields.at(2).toDouble();
    data.altitude = fields.at(3).toDouble();
    data.bearing = fields.at(4).toDouble();
    data.speed = fields.at(5).toDouble();
    data.brake = fields.at(6).toDouble();
    data.steeringAngle = fields.at(7).toDouble();
    data.throttle = fields.at(8).toDouble();
    data.rpm = fields.at(9).toDouble();
    data.yaw = fields.at(10).toDouble();
    data.latg = fields.at(11).toDouble();

    if (!m_sequence.isEmpty()) { // interpolate to 60 fps
      Model::Data prev_data = m_sequence.last();
      qint64 diff_timestamp = data.timestamp - prev_data.timestamp;
      static const qint64 frame_rate = 1000 / 60;
      int additional_frame_count = diff_timestamp / frame_rate;
      double divider = additional_frame_count;

      if (additional_frame_count > 0) {
        diff_timestamp = static_cast<double>(diff_timestamp) / divider;
        double diff_latitude = (data.latitude - prev_data.latitude) / divider;
        double diff_longitude = (data.longitude - prev_data.longitude) / divider;
        double diff_altitude = (data.altitude - prev_data.altitude) / divider;
        double diff_bearing = (data.bearing - prev_data.bearing) / divider;
        double diff_speed = (data.speed - prev_data.speed) / divider;
        double diff_brake = (data.brake - prev_data.brake) / divider;
        double diff_steeringAngle = (data.steeringAngle - prev_data.steeringAngle) / divider;
        double diff_throttle = (data.throttle - prev_data.throttle) / divider;
        double diff_rpm = (data.rpm - prev_data.rpm) / divider;
        double diff_yaw = (data.yaw - prev_data.yaw) / divider;
        double diff_latg = (data.latg - prev_data.latg) / divider;

        for (int i = 0; i < additional_frame_count - 1; ++i) {
          prev_data.timestamp += diff_timestamp;
          prev_data.latitude += diff_latitude;
          prev_data.longitude += diff_longitude;
          prev_data.altitude += diff_altitude;
          prev_data.bearing += diff_bearing;
          prev_data.speed += diff_speed;
          prev_data.brake += diff_brake;
          prev_data.steeringAngle += diff_steeringAngle;
          prev_data.throttle += diff_throttle;
          prev_data.rpm += diff_rpm;
          prev_data.yaw += diff_yaw;
          prev_data.latg += diff_latg;
          m_sequence << prev_data;
        }
      }
    }

    m_sequence << data;
  }

  m_size = m_sequence.size();
  emit sizeChanged(m_size);
  setIndex(0);

  emit status(tr("Succes!"));
}

void CSVParser::next()
{
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index + 1);
  if (m_playing) {
    if (m_index < m_sequence.size() - 1) {
      QTimer::singleShot(m_sequence.at(m_index + 1).timestamp - m_model->timestamp(), this, SLOT(next()));
    }
  }

  emit indexChanged(m_index);
}

void CSVParser::prev()
{
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index - 1);

  emit indexChanged(m_index);
}

void CSVParser::playPause()
{
  m_playing = !m_playing;
  if (m_playing) {
    next();
  }
}

Model *CSVParser::model() const
{
  return m_model;
}

int CSVParser::index() const
{
  return m_index;
}

int CSVParser::size() const
{
  return m_size;
}

void CSVParser::setIndex(int index)
{
  if (index > size() - 1)
    index = size() - 1;

  if (index < 0)
    index = 0;

  if (m_index == index && index != 0)
    return;

  m_index = index;
  emit modelChanged(m_model);

  m_model->setData(m_sequence.at(m_index));

  emit indexChanged(m_index);
}
