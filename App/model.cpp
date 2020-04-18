#include "model.h"

Model::Model(QObject *parent) : QObject(parent) {}

qint64 Model::timestamp() const { return m_data.timestamp; }

double Model::latitude() const { return m_data.latitude; }

double Model::longitude() const { return m_data.longitude; }

double Model::altitude() const { return m_data.altitude; }

double Model::bearing() const { return m_data.bearing; }

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

void Model::setLatitude(double latitude) {
  if (qFuzzyCompare(m_data.latitude, latitude))
    return;

  m_data.latitude = latitude;
  emit latitudeChanged(m_data.latitude);
}

void Model::setLongitude(double longitude) {
  if (qFuzzyCompare(m_data.longitude, longitude))
    return;

  m_data.longitude = longitude;
  emit longitudeChanged(m_data.longitude);
}

void Model::setAltitude(double altitude) {
  if (qFuzzyCompare(m_data.altitude, altitude))
    return;

  m_data.altitude = altitude;
  emit altitudeChanged(m_data.altitude);
}

void Model::setBearing(double bearing) {
  if (qFuzzyCompare(m_data.bearing, bearing))
    return;

  m_data.bearing = bearing;
  emit bearingChanged(m_data.bearing);
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
