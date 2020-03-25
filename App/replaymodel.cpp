#include "replaymodel.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QTimer>

ReplayModel::ReplayModel(QObject *parent) : QObject(parent) {}

QUrl ReplayModel::directory() {
  QStringList locations =
      QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
  return locations.isEmpty() ? QUrl{} : QUrl::fromLocalFile(locations.first());
}

QString ReplayModel::exportDirectory() {
  QDir dir;
  dir.mkdir(exportDir);
  return exportDir;
}

void ReplayModel::loadUrl(const QUrl &url) {
  m_sequence.clear();

  QFile file(url.toLocalFile());
  if (!file.open(QFile::ReadOnly)) {
    emit error(tr("Could not open file. '%1'").arg(file.errorString()));
    return;
  }

  exportDir = QString("%1.frames").arg(file.fileName());

  QTextStream stream(&file);
  stream.setCodec("utf-8");

  bool skip_header = true;
  while (!stream.atEnd()) {
    QStringList fields = stream.readLine().split(',');
    if (skip_header) {
      skip_header = false;
      continue;
    }

    if (fields.size() != 8) {
      emit error(tr("Invalid row count '%1'").arg(fields.join(",")));
      return;
    }

    Model::Data data;
    data.timestamp = fields.at(0).toULongLong();
    data.speed = fields.at(1).toDouble();
    data.brake = fields.at(2).toDouble();
    data.steeringAngle = fields.at(3).toDouble();
    data.throttle = fields.at(4).toDouble();
    data.rpm = fields.at(5).toDouble();
    data.yaw = fields.at(6).toDouble();
    data.latg = fields.at(7).toDouble();

    if (!m_sequence.isEmpty()) { // interpolate to 60 fps
      Model::Data prev_data = m_sequence.last();
      qint64 diff_timestamp = data.timestamp - prev_data.timestamp;
      static const qint64 frame_rate = 1000 / 60;
      int additional_frame_count = diff_timestamp / frame_rate;
      double divider = additional_frame_count;

      if (additional_frame_count > 0) {
        diff_timestamp = static_cast<double>(diff_timestamp) / divider;
        double diff_speed = (data.speed - prev_data.speed) / divider;
        double diff_brake = (data.brake - prev_data.brake) / divider;
        double diff_steeringAngle =
            (data.steeringAngle - prev_data.steeringAngle) / divider;
        double diff_throttle = (data.throttle - prev_data.throttle) / divider;
        double diff_rpm = (data.rpm - prev_data.rpm) / divider;
        double diff_yaw = (data.yaw - prev_data.yaw) / divider;
        double diff_latg = (data.latg - prev_data.latg) / divider;

        for (int i = 0; i < additional_frame_count - 1; ++i) {
          prev_data.timestamp += diff_timestamp;
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
  emit sizeChanged(m_size);
  m_model.setData(m_sequence.at(m_index));
  emit modelChanged();

  emit status(tr("Succes!"));
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
