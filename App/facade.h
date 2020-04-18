#ifndef FACADE_H
#define FACADE_H

#include <QObject>

#include "datalogger.h"
#include "ds2message.h"
#include "location.h"
#include "requester.h"

class Facade : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT)
  Q_PROPERTY(DataLogger *logger READ logger CONSTANT)
  Q_PROPERTY(int delay READ delay WRITE setDelay NOTIFY delayChanged)
  Q_PROPERTY(int latency READ latency WRITE setLatency NOTIFY latencyChanged)
  Q_PROPERTY(bool hasLocation READ hasLocation CONSTANT)

  Q_PROPERTY(
      int throttleAndRPMFrequency READ throttleAndRPMFrequency WRITE
          setThrottleAndRPMFrequency NOTIFY throttleAndRPMFrequencyChanged)
  Q_PROPERTY(int speedFrequency READ speedFrequency WRITE setSpeedFrequency
                 NOTIFY speedFrequencyChanged)
  Q_PROPERTY(int dscOffsetsFrequency READ dscOffsetsFrequency WRITE
                 setDscOffsetsFrequency NOTIFY dscOffsetsFrequencyChanged)
  Q_PROPERTY(
      int dscSteeringAngleFrequency READ dscSteeringAngleFrequency WRITE
          setDscSteeringAngleFrequency NOTIFY dscSteeringAngleFrequencyChanged)
  Q_PROPERTY(
      int dscBrakeYawLatgFrequency READ dscBrakeYawLatgFrequency WRITE
          setDscBrakeYawLatgFrequency NOTIFY dscBrakeYawLatgFrequencyChanged)

public:
  explicit Facade(QObject *parent = nullptr);

  Model *model();
  DataLogger *logger();
  int delay() const;
  int latency() const;

  int throttleAndRPMFrequency() const;
  int speedFrequency() const;
  int dscOffsetsFrequency() const;
  int dscSteeringAngleFrequency() const;
  int dscBrakeYawLatgFrequency() const;
  bool hasLocation() const;

signals:
  void sendData(const QByteArray &data);
  void delayChanged(int delay);
  void latencyChanged(int latency);

  void throttleAndRPMFrequencyChanged(int throttleAndRPMFrequency);
  void speedFrequencyChanged(int speedFrequency);
  void dscOffsetsFrequencyChanged(int dscOffsetsFrequency);
  void dscSteeringAngleFrequencyChanged(int dscSteeringAngleFrequency);
  void dscBrakeYawLatgFrequencyChanged(int dscBrakeYawLatgFrequency);

public slots:
  void sendRequest();
  void dataReceived(const QByteArray &data);
  void connected();
  void setDelay(int delay);
  void setLatency(int latency);

  void setThrottleAndRPMFrequency(int throttleAndRPMFrequency);
  void setSpeedFrequency(int speedFrequency);
  void setDscOffsetsFrequency(int dscOffsetsFrequency);
  void setDscSteeringAngleFrequency(int dscSteeringAngleFrequency);
  void setDscBrakeYawLatgFrequency(int dscBrakeYawLatgFrequency);

private:
  void chooseNextRequester();

private:
  std::shared_ptr<Model> m_model;
  size_t index{};
  QByteArray buffer;
  DS2Parser parser;
  DataLogger m_logger;
  int m_delay{};
  int m_latency{};
  qint64 last_response{};

  int m_throttleAndRPMFrequency{1};
  int m_speedFrequency{0};
  int m_dscOffsetsFrequency{20};
  int m_dscSteeringAngleFrequency{1};
  int m_dscBrakeYawLatgFrequency{1};

  std::vector<Requester> requesters;
  Location location;
  bool m_first_response{true};
  bool m_hasLocation{false};
};

#endif // FACADE_H
