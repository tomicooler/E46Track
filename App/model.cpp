#include "model.h"

Model::Model(QObject *parent) : QObject(parent) {}

qint64 Model::timestamp() const { return m_data.timestamp; }

double Model::speed() const { return m_data.speed; }

double Model::brake() const { return m_data.brake; }

double Model::steeringAngle() const { return m_data.steeringAngle; }

double Model::throttle() const { return m_data.throttle; }

double Model::rpm() const { return m_data.rpm; }

double Model::yaw() const { return m_data.yaw; }

double Model::latg() const { return m_data.latg; }

Model::Data Model::data() const { return m_data; }

void Model::setTimestamp(qint64 timestamp) {
  if (m_data.timestamp == timestamp)
    return;

  m_data.timestamp = timestamp;
  emit timestampChanged(m_data.timestamp);
}

void Model::setSpeed(double speed) {
  if (qFuzzyCompare(m_data.speed, speed))
    return;

  m_data.speed = speed;
  emit speedChanged(m_data.speed);
}

void Model::setBrake(double brake) {
  if (qFuzzyCompare(m_data.brake, brake))
    return;

  m_data.brake = brake;
  emit brakeChanged(m_data.brake);
}

void Model::setSteeringAngle(double steeringAngle) {
  if (qFuzzyCompare(m_data.steeringAngle, steeringAngle))
    return;

  m_data.steeringAngle = steeringAngle;
  emit steeringAngleChanged(m_data.steeringAngle);
}

void Model::setThrottle(double throttle) {
  if (qFuzzyCompare(m_data.throttle, throttle))
    return;

  m_data.throttle = throttle;
  emit throttleChanged(m_data.throttle);
}

void Model::setRpm(double rpm) {
  if (qFuzzyCompare(m_data.rpm, rpm))
    return;

  m_data.rpm = rpm;
  emit rpmChanged(m_data.rpm);
}

void Model::setYaw(double yaw) {
  if (qFuzzyCompare(m_data.yaw, yaw))
    return;

  m_data.yaw = yaw;
  emit yawChanged(m_data.yaw);
}

void Model::setLatg(double latg) {
  if (qFuzzyCompare(m_data.latg, latg))
    return;

  m_data.latg = latg;
  emit latgChanged(m_data.latg);
}

void Model::setData(const Model::Data &data) {
  setTimestamp(data.timestamp);
  setSpeed(data.speed);
  setBrake(data.brake);
  setSteeringAngle(data.steeringAngle);
  setThrottle(data.throttle);
  setRpm(data.rpm);
  setYaw(data.yaw);
  setLatg(data.latg);
}
