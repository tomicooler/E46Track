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
                    m_model},
          Requester{DS2Message{QByteArray::fromHex("12"),
                               QByteArray::fromHex("0b13")},
                    {std::make_shared<Speed>()},
                    m_model},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2102")},
                    {}, // DSC offsets, must be sent
                    m_model},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2201f5")},
                    {std::make_shared<SteeringAngle>()},
                    m_model},
          Requester{DS2Message{QByteArray::fromHex("b829f1"),
                               QByteArray::fromHex("2106")},
                    {std::make_shared<Brake>(), std::make_shared<Yaw>(),
                     std::make_shared<LatG>()},
                    m_model}} {
  logger()->setModel(m_model);
}

Model *Facade::model() { return m_model.get(); }

DataLogger *Facade::logger() { return &m_logger; }

int Facade::delay() const { return m_delay; }

int Facade::latency() const { return m_latency; }

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

        ++index;
        if (index >= requesters.size()) {
          index = 0;
        }

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
