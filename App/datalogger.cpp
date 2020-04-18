#include "datalogger.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

class FileLogger {
public:
  explicit FileLogger(const QString &path) : file(path), stream(&file) {
    if (!file.open(QFile::WriteOnly)) {
      qDebug() << path << file.errorString();
    }
    qDebug() << "path" << path;
    stream.setCodec("utf-8");
    stream << "timestamp (milliseconds since epoch),speed (kmph),brake "
              "(bar),steeringAngle (degree),throttle (V),rpm,yaw "
              "(degree/sec),latg (g)\n";
  }

  QFile file;
  QTextStream stream;
};

DataLogger::DataLogger(QObject *parent) : QObject(parent) {
  setStartTime(QDateTime::currentMSecsSinceEpoch());
}

DataLogger::~DataLogger() {}

bool DataLogger::logging() const { return m_logging; }

qint64 DataLogger::startTime() const { return m_startTime; }

qint64 DataLogger::elapsedTime() const { return m_elapsedTime; }

void DataLogger::setModel(const std::shared_ptr<Model> &model) {
  this->model = model;
}

void DataLogger::setHasLocation(bool hasLocation) {
  this->m_hasLocation = hasLocation;
}

void DataLogger::setLogging(bool logging) {
  if (m_logging == logging)
    return;

  if (logging) {
    setStartTime(QDateTime::currentMSecsSinceEpoch());
    setElapsedTime(0);
    QStringList locations =
        QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    QString location = locations.isEmpty() ? "/tmp" : locations.first();
    QDir dir;
    dir.mkpath(location);
    logger = std::make_unique<FileLogger>(
        QString("%1/e46track_%2.csv")
            .arg(location)
            .arg(QDateTime::fromMSecsSinceEpoch(m_startTime)
                     .toString("yyyy-MM-dd_hh:mm:ss")));
    connect(model.get(), &Model::updated, this, &DataLogger::log);
    elapsed.start();
  } else {
    disconnect(model.get(), &Model::updated, this, &DataLogger::log);
    logger.reset();
  }

  m_logging = logging;
  emit loggingChanged(m_logging);
}

void DataLogger::setStartTime(qint64 startTime) {
  if (m_startTime == startTime)
    return;

  m_startTime = startTime;
  emit startTimeChanged(m_startTime);
}

void DataLogger::setElapsedTime(qint64 elapsedTime) {
  if (m_elapsedTime == elapsedTime)
    return;

  m_elapsedTime = elapsedTime;
  emit elapsedTimeChanged(m_elapsedTime);
}

void DataLogger::log() {
  qint64 timestamp = m_startTime + elapsed.elapsed();
  logger->stream << QString("%1,%2,%3,%4,%5,%6,%7,%8\n")
                        .arg(timestamp)
                        .arg(model.get()->speed(), 0, 'f', 2)
                        .arg(model.get()->brake(), 0, 'f', 2)
                        .arg(model.get()->steeringAngle(), 0, 'f', 2)
                        .arg(model.get()->throttle(), 0, 'f', 4)
                        .arg(model.get()->rpm(), 0, 'f', 0)
                        .arg(model.get()->yaw(), 0, 'f', 4)
                        .arg(model.get()->latg(), 0, 'f', 4);
  logger->stream.flush();
  setElapsedTime(elapsed.elapsed());
}
