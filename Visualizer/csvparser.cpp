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

  bool first = true;
  while (!stream.atEnd()) {
    QStringList fields = stream.readLine().split(',');
    if (first) {
      first = false;
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

    m_sequence << data;
  }

  m_size = m_sequence.size();
  emit sizeChanged(m_size);
  m_index = 0;
  emit indexChanged(m_index);

  emit status(tr("Succes!"));
}

void CSVParser::next()
{
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index + 1);

  emit indexChanged(m_index);
}

void CSVParser::prev()
{
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index - 1);

  emit indexChanged(m_index);
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

  if (m_index == index)
    return;

  m_model->setData(m_sequence.at(m_index));
  emit modelChanged(m_model);

  m_index = index;
  emit indexChanged(m_index);
}
