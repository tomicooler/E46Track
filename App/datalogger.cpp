#include "datalogger.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

class FileLogger {
public:
  explicit FileLogger(const QString &path, bool hasLocation)
      : file(path), stream(&file) {
    if (!file.open(QFile::WriteOnly)) {
      qDebug() << path << file.errorString();
    }
    qDebug() << "path" << path << "haslocation" << hasLocation;
    stream.setCodec("utf-8");
    stream << "timestamp (milliseconds since epoch)";
    if (hasLocation) {
      stream << ",latitude,longitude,altitude,bearing";
    }
    stream << ",speed (kmph),brake "
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
                     .toString("yyyy-MM-dd_hh:mm:ss")),
        m_hasLocation);
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
  logger->stream << QString("%1,%2%3,%4,%5,%6,%7,%8,%9\n")
                        .arg(timestamp)
                        .arg(m_hasLocation
                                 ? QString("%1,%2,%3,%4,")
                                       .arg(model->latitude(), 0, 'f', 10)
                                       .arg(model->longitude(), 0, 'f', 10)
                                       .arg(model->altitude(), 0, 'f', 10)
                                       .arg(model->bearing(), 0, 'f', 10)
                                 : QString())
                        .arg(model->speed(), 0, 'f', 2)
                        .arg(model->brake(), 0, 'f', 2)
                        .arg(model->steeringAngle(), 0, 'f', 2)
                        .arg(model->throttle(), 0, 'f', 4)
                        .arg(model->rpm(), 0, 'f', 0)
                        .arg(model->yaw(), 0, 'f', 4)
                        .arg(model->latg(), 0, 'f', 4);
  logger->stream.flush();
  setElapsedTime(elapsed.elapsed());
}
