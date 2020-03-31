#ifndef FACADE_H
#define FACADE_H

#include <QObject>

#include "datalogger.h"
#include "ds2message.h"
#include "requester.h"

class Facade : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT)
  Q_PROPERTY(DataLogger *logger READ logger CONSTANT)
  Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged)
  Q_PROPERTY(int latency READ latency WRITE setLatency NOTIFY latencyChanged)

public:
  explicit Facade(QObject *parent = nullptr);

  Model *model();
  DataLogger *logger();
  int delay() const;
  int latency() const;

signals:
  void sendData(const QByteArray &data);
  void delayChanged(int delay);
  void latencyChanged(int latency);

public slots:
  void sendRequest();
  void dataReceived(const QByteArray &data);
  void connected();
  void setDelay(int delay);
  void setLatency(int latency);

private:
  std::shared_ptr<Model> m_model;
  std::vector<Requester> requesters;
  size_t index{};
  QByteArray buffer;
  DS2Parser parser;
  DataLogger m_logger;
  int m_delay{};
  int m_latency{};
  qint64 last_response{};
};

#endif // FACADE_H
