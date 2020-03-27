#ifndef EXTRACTORS_H
#define EXTRACTORS_H

#include <QByteArray>

#include "ds2message.h"
#include "model.h"

class SignedTwoByteExtractor {
public:
  double extract(const QByteArray &data, int pos, double multiplier = 1.0,
                 double offset = 0.0) const {
    // The data is not a 16 bit signed two's complement integer.
    // ( != little_endian short )
    int number = ((data.at(pos) & 0xFF) + ((data.at(pos + 1) & 0xFF) << 8));
    number = (number & 0x7FFF) * ((number & 0x8000) > 0 ? -1 : 1);
    return (number * multiplier) + offset;
  }
};

class SignedTwoStepByteExtractor {
public:
  double extract(const QByteArray &data, int pos, double multiplier = 1.0,
                 double offset = 0.0) const {
    return (((static_cast<qint8>(data.at(pos)) * 2.56) +
             (static_cast<quint8>(data.at(pos + 1)) / 100.0))) *
               multiplier +
           offset;
  }
};

class ModelUpdater {
public:
  virtual ~ModelUpdater() = default;
  virtual void update(const DS2Message &message, Model &model) const = 0;
};

class Brake : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setBrake(extractor.extract(message.data, 6));
  }

private:
  const SignedTwoStepByteExtractor extractor{};
};

class Throttle : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setThrottle(extractor.extract(message.data, 24, 0.295, 0.75));
  }

private:
  const SignedTwoStepByteExtractor extractor{};
};

class RPM : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setRpm(extractor.extract(message.data, 1, 100.0));
  }

private:
  const SignedTwoStepByteExtractor extractor{};
};

class SteeringAngle : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setSteeringAngle(extractor.extract(message.data, 4, 0.045));
  }

private:
  const SignedTwoByteExtractor extractor{};
};

class Yaw : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setYaw(extractor.extract(message.data, 10, 0.285));
  }

private:
  const SignedTwoStepByteExtractor extractor{};
};

class LatG : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setLatg(extractor.extract(message.data, 12, 0.1));
  }

private:
  const SignedTwoStepByteExtractor extractor{};
};

class Speed : public ModelUpdater {
public:
  void update(const DS2Message &message, Model &model) const override {
    model.setSpeed(static_cast<quint8>(message.data.at(2)));
  }
};

#endif // EXTRACTORS_H
