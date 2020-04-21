#include "replaymodel.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QQmlFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>

#include <cmath>

ReplayModel::ReplayModel(QObject *parent) : QObject(parent) {}

QUrl ReplayModel::directory() {
  QStringList locations =
      QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
  return locations.isEmpty() ? QUrl{} : QUrl::fromLocalFile(locations.first());
}

QString ReplayModel::exportDirectory() {
  if (!exportDir.isEmpty()) {
    QDir dir;
    dir.mkdir(exportDir);
  }
  return exportDir;
}

void ReplayModel::loadUrl(const QUrl &url) {
  m_index = 0;
  m_size = 0;
  m_playing = false;
  emit indexChanged(m_index);
  emit sizeChanged(m_size);
  exportDir.clear();
  m_sequence.clear();

  QFile file(QQmlFile::urlToLocalFileOrQrc(url));
  if (!file.open(QFile::ReadOnly)) {
    emit error(tr("Could not open file. '%1'").arg(file.errorString()));
    return;
  }

  exportDir = QString("%1/e46track_export_%2")
                  .arg(directory().toLocalFile())
                  .arg(QDateTime::fromMSecsSinceEpoch(
                           QDateTime::currentMSecsSinceEpoch())
                           .toString("yyyy-MM-dd_hh:mm:ss"));

  QTextStream stream(&file);
  stream.setCodec("utf-8");

  bool skip_header = true;
  m_hasSpeed = false;
  while (!stream.atEnd()) {
    QStringList fields = stream.readLine().split(',');
    if (skip_header) {
      skip_header = false;
      continue;
    }

    if (fields.size() != 8 &&
        fields.size() != 12) { // ugly but good enough for me :)
      emit error(tr("Invalid row count '%1'").arg(fields.join(",")));
      return;
    }

    Model::Data data;
    data.timestamp = fields.at(0).toULongLong();
    if (fields.size() == 12) {
      data.latitude = fields.at(1).toDouble();
      data.longitude = fields.at(2).toDouble();
      data.altitude = fields.at(3).toDouble();
      data.bearing = fields.at(4).toDouble();
    }
    int speedShift = fields.size() == 8 ? 0 : 4;
    data.speed = fields.at(1 + speedShift).toDouble();
    data.brake = fields.at(2 + speedShift).toDouble();
    data.steeringAngle = fields.at(3 + speedShift).toDouble();
    data.throttle = fields.at(4 + speedShift).toDouble();
    data.rpm = fields.at(5 + speedShift).toDouble();
    data.yaw = fields.at(6 + speedShift).toDouble();
    data.latg = fields.at(7 + speedShift).toDouble();

    m_hasSpeed = m_hasSpeed || data.speed > 0.0;

    if (!m_sequence.isEmpty()) { // interpolate to 60 fps
      Model::Data prev_data = m_sequence.last();
      double diff_timestamp = data.timestamp - prev_data.timestamp;
      static const double frame_rate = 1000.0 / 60.0;
      double divider = diff_timestamp / frame_rate;
      int additional_frame_count = std::round(divider) - 1;

      if (additional_frame_count > 0) {
        diff_timestamp /= divider;

        // When location updates are slower than the other data upate rate, the
        // interpolation won't be accurate for the location data. E.g: my phone
        // max update rate on location is 1 sec, a tipical roundtrip for inpa
        // data is 400-500 ms.. Anyway this is a PoC forr location updates.
        double diff_latitude = (data.latitude - prev_data.latitude) / divider;
        double diff_longitude =
            (data.longitude - prev_data.longitude) / divider;
        double diff_altitude = (data.altitude - prev_data.altitude) / divider;
        double diff_bearing = (data.bearing - prev_data.bearing) / divider;

        double diff_speed = (data.speed - prev_data.speed) / divider;
        double diff_brake = (data.brake - prev_data.brake) / divider;
        double diff_steeringAngle =
            (data.steeringAngle - prev_data.steeringAngle) / divider;
        double diff_throttle = (data.throttle - prev_data.throttle) / divider;
        double diff_rpm = (data.rpm - prev_data.rpm) / divider;
        double diff_yaw = (data.yaw - prev_data.yaw) / divider;
        double diff_latg = (data.latg - prev_data.latg) / divider;

        for (int i = 0; i < additional_frame_count; ++i) {
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

  if (m_sequence.isEmpty()) {
    emit error(tr("No valid data in file='%1'").arg(file.fileName()));
    return;
  }

  m_size = m_sequence.size();
  emit hasSpeedChanged(m_hasSpeed);
  emit sizeChanged(m_size);
  m_model.setData(m_sequence.at(m_index));
  emit modelChanged();
}

void ReplayModel::next() {
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index + 1);
  if (m_playing) {
    if (m_index < m_sequence.size() - 1) {
      QTimer::singleShot(m_sequence.at(m_index + 1).timestamp -
                             m_model.timestamp(),
                         this, SLOT(next()));
    } else {
      m_playing = false;
    }
  }

  emit indexChanged(m_index);
}

void ReplayModel::prev() {
  if (m_sequence.isEmpty())
    return;

  setIndex(m_index - 1);

  emit indexChanged(m_index);
}

void ReplayModel::playPause() {
  m_playing = !m_playing;
  if (m_playing) {
    next();
  }
}

Model *ReplayModel::model() { return &m_model; }

int ReplayModel::index() const { return m_index; }

int ReplayModel::size() const { return m_size; }

bool ReplayModel::hasSpeed() const { return m_hasSpeed; }

void ReplayModel::setIndex(int index) {
  if (index > size() - 1)
    index = size() - 1;

  if (index < 0)
    index = 0;

  if (m_index == index)
    return;

  m_index = index;

  m_model.setData(m_sequence.at(m_index));

  emit indexChanged(m_index);
  emit modelChanged();
}
