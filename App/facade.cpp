#include "facade.h"

#include <QDateTime>
#include <QDebug>
#include <QTimer>

namespace {

static const inline QByteArray acknowledgement = QByteArray::fromHex("a0");
static const inline QByteArray dscResponse = QByteArray::fromHex("b8f129");

bool isResponse(const DS2Message &message) {
  return message.data.startsWith(QByteArray::fromHex("a0")) ||
         message.ecu.startsWith(QByteArray::fromHex("b8f129"));
}

} // namespace

Facade::Facade(QObject *parent)
    : QObject(parent), m_model(std::make_shared<Model>()),
      requesters{
          Requester{DS2Message{QByteArray::fromHex("12"),
                               QByteArray::fromHex("0b03")},
                    {std::make_shared<Throttle>(), std::make_shared<RPM>()},
                    m_model,
                    m_throttleAndRPMFrequency},
          Requester{DS2Message{QByteArray::fromHex("12"),
                               QByteArray::fromHex("0b13")},
                    {std::make_shared<Speed>()},
                    m_model,
                    m_speedFrequency},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2102")},
                    {}, // DSC offsets, must be sent
                    m_model,
                    m_dscOffsetsFrequency},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2201f5")},
                    {std::make_shared<SteeringAngle>()},
                    m_model,
                    m_dscSteeringAngleFrequency},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2106")},
                    {std::make_shared<Brake>(), std::make_shared<Yaw>(),
                     std::make_shared<LatG>()},
                    m_model,
                    m_dscBrakeYawLatgFrequency}},
      location(m_model) {
  logger()->setModel(m_model);
  logger()->setHasLocation(location.hasLocation());
  qDebug() << "haslocation" << location.hasLocation();
}

Model *Facade::model() { return m_model.get(); }

DataLogger *Facade::logger() { return &m_logger; }

int Facade::delay() const { return m_delay; }

int Facade::latency() const { return m_latency; }

int Facade::throttleAndRPMFrequency() const {
  return m_throttleAndRPMFrequency;
}

int Facade::speedFrequency() const { return m_speedFrequency; }

int Facade::dscOffsetsFrequency() const { return m_dscOffsetsFrequency; }

int Facade::dscSteeringAngleFrequency() const {
  return m_dscSteeringAngleFrequency;
}

int Facade::dscBrakeYawLatgFrequency() const {
  return m_dscBrakeYawLatgFrequency;
}

void Facade::dataReceived(const QByteArray &data) {
  if (index >= requesters.size())
    return;

  buffer += data;
  if (buffer.size() > 255) {
    buffer.clear();
  }

  do {
    if (const auto message = parser.parse(buffer); message.has_value()) {
      if (isResponse(message.value())) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        setLatency(now - last_response);
        last_response = now;

        requesters.at(index).processResponse(message.value());

        chooseNextRequester();
        QTimer::singleShot(delay(), this, &Facade::sendRequest);
      }
    } else {
      break;
    }
  } while (buffer.size() > 0);
}

void Facade::sendRequest() {
  if (index >= requesters.size())
    return;

  emit sendData(requesters.at(index).get_message().serialized);
}

void Facade::connected() {
  last_response = QDateTime::currentMSecsSinceEpoch();
  chooseNextRequester();
  QTimer::singleShot(100, this, &Facade::sendRequest);
}

void Facade::setDelay(int delay) {
  if (m_delay == delay)
    return;

  m_delay = delay;
  emit delayChanged(m_delay);
}

void Facade::setLatency(int latency) {
  if (m_latency == latency)
    return;

  m_latency = latency;
  emit latencyChanged(m_latency);
}

void Facade::setThrottleAndRPMFrequency(int throttleAndRPMFrequency) {
  if (m_throttleAndRPMFrequency == throttleAndRPMFrequency)
    return;

  m_throttleAndRPMFrequency = throttleAndRPMFrequency;
  requesters.at(0).setFrequency(m_throttleAndRPMFrequency);
  emit throttleAndRPMFrequencyChanged(m_throttleAndRPMFrequency);
}

void Facade::setSpeedFrequency(int speedFrequency) {
  if (m_speedFrequency == speedFrequency)
    return;

  m_speedFrequency = speedFrequency;
  requesters.at(1).setFrequency(m_speedFrequency);
  emit speedFrequencyChanged(m_speedFrequency);
}

void Facade::setDscOffsetsFrequency(int dscOffsetsFrequency) {
  if (m_dscOffsetsFrequency == dscOffsetsFrequency)
    return;

  m_dscOffsetsFrequency = dscOffsetsFrequency;
  requesters.at(2).setFrequency(m_dscOffsetsFrequency);
  emit dscOffsetsFrequencyChanged(m_dscOffsetsFrequency);
}

void Facade::setDscSteeringAngleFrequency(int dscSteeringAngleFrequency) {
  if (m_dscSteeringAngleFrequency == dscSteeringAngleFrequency)
    return;

  m_dscSteeringAngleFrequency = dscSteeringAngleFrequency;
  requesters.at(3).setFrequency(m_dscSteeringAngleFrequency);
  emit dscSteeringAngleFrequencyChanged(m_dscSteeringAngleFrequency);
}

void Facade::setDscBrakeYawLatgFrequency(int dscBrakeYawLatgFrequency) {
  if (m_dscBrakeYawLatgFrequency == dscBrakeYawLatgFrequency)
    return;

  m_dscBrakeYawLatgFrequency = dscBrakeYawLatgFrequency;
  requesters.at(4).setFrequency(m_dscBrakeYawLatgFrequency);
  emit dscBrakeYawLatgFrequencyChanged(m_dscBrakeYawLatgFrequency);
}

void Facade::chooseNextRequester() {
  while (true) {
    ++index;
    if (index >= requesters.size()) {
      emit m_model->updated();
      index = 0;
    }

    if (requesters.at(index).shouldRequest())
      break;
  }
}
