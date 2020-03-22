#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject {
public:
  struct Data {
    qint64 timestamp{};
    double speed{};
    double brake{};
    double steeringAngle{};
    double throttle{};
    double rpm{};
    double yaw{};
    double latg{};
  };

private:
  Q_OBJECT

  Q_PROPERTY(qint64 timestamp READ timestamp WRITE setTimestamp NOTIFY
                 timestampChanged)

  Q_PROPERTY(double speed READ speed WRITE setSpeed NOTIFY speedChanged)
  Q_PROPERTY(double brake READ brake WRITE setBrake NOTIFY brakeChanged)
  Q_PROPERTY(double steeringAngle READ steeringAngle WRITE setSteeringAngle
                 NOTIFY steeringAngleChanged)
  Q_PROPERTY(
      double throttle READ throttle WRITE setThrottle NOTIFY throttleChanged)
  Q_PROPERTY(double rpm READ rpm WRITE setRpm NOTIFY rpmChanged)
  Q_PROPERTY(double yaw READ yaw WRITE setYaw NOTIFY yawChanged)
  Q_PROPERTY(double latg READ latg WRITE setLatg NOTIFY latgChanged)

public:
  explicit Model(QObject *parent = nullptr);

  qint64 timestamp() const;
  double speed() const;
  double brake() const;
  double steeringAngle() const;
  double throttle() const;
  double rpm() const;
  double yaw() const;
  double latg() const;
  Model::Data data() const;

public slots:
  void setTimestamp(qint64 timestamp);
  void setSpeed(double speed);
  void setBrake(double brake);
  void setSteeringAngle(double steeringAngle);
  void setThrottle(double throttle);
  void setRpm(double rpm);
  void setYaw(double yaw);
  void setLatg(double latg);
  void setData(const Data &data);

signals:
  void timestampChanged(qint64 timestamp);
  void speedChanged(double speed);
  void brakeChanged(double brake);
  void steeringAngleChanged(double steeringAngle);
  void throttleChanged(double throttle);
  void rpmChanged(double rpm);
  void yawChanged(double yaw);
  void latgChanged(double latg);

private:
  Data m_data;
};

#endif // MODEL_H
