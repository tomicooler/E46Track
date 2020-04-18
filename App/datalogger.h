#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QElapsedTimer>
#include <QObject>

#include <memory>

#include "model.h"

class DataLogger : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool logging READ logging WRITE setLogging NOTIFY loggingChanged)
  Q_PROPERTY(qint64 startTime READ startTime WRITE setStartTime NOTIFY
                 startTimeChanged)
  Q_PROPERTY(qint64 elapsedTime READ elapsedTime WRITE setElapsedTime NOTIFY
                 elapsedTimeChanged)

public:
  explicit DataLogger(QObject *parent = nullptr);
  ~DataLogger() override;

  bool logging() const;
  qint64 startTime() const;
  qint64 elapsedTime() const;

  void setModel(const std::shared_ptr<Model> &model);
  void setHasLocation(bool hasLocation);

public slots:
  void setLogging(bool logging);
  void setStartTime(qint64 startTime);
  void setElapsedTime(qint64 elapsedTime);

signals:
  void loggingChanged(bool logging);
  void startTimeChanged(qint64 startTime);
  void elapsedTimeChanged(qint64 elapsedTime);

private slots:
  void log();

private:
  bool m_logging{};
  bool m_hasLocation{};
  qint64 m_startTime{};
  qint64 m_elapsedTime{};
  std::shared_ptr<Model> model;
  std::unique_ptr<class FileLogger> logger;
  QElapsedTimer elapsed;
};

#endif // DATALOGGER_H
