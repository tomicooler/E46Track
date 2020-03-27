#include "facade.h"

#include <QTimer>

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

void Facade::dataReceived(const QByteArray &data) {
  if (index >= requesters.size())
    return;

  buffer += data;
  if (buffer.size() > 255) {
    buffer.clear();
  }

  if (const auto message = parser.parse(buffer); message.has_value()) {
    requesters.at(index).processResponse(message.value());
  }

  ++index;
  if (index >= requesters.size()) {
    index = 0;
  }

  QTimer::singleShot(20, this, &Facade::sendRequest);
}

void Facade::sendRequest() {
  if (index >= requesters.size())
    return;

  emit sendData(requesters.at(index).get_message().serialized);
}

void Facade::connected() { sendRequest(); }
