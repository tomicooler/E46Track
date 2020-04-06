#ifndef REQUESTER_H
#define REQUESTER_H

#include <QList>
#include <memory>
#include <vector>

#include "extractors.h"

using ModelUpdaterPtr = std::shared_ptr<ModelUpdater>;
using ModelUpdaters = std::vector<ModelUpdaterPtr>;

class Requester {
public:
  explicit Requester(const DS2Message &message, ModelUpdaters &&updaters,
                     const std::shared_ptr<Model> &model, int frequency)
      : message(message), updaters(updaters), model(model), counter(frequency),
        frequency(frequency) {}

  void processResponse(const DS2Message &response) {
    for (const auto &updater : updaters)
      updater->update(response, *model);
  }

  bool shouldRequest() {
    ++counter;
    return frequency == 0 ? false : counter % frequency == 0;
  }

  void setFrequency(int frequency) {
    this->counter = frequency; // first shouldRequest should return true
    this->frequency = frequency;
  }

  const DS2Message &get_message() { return message; }

private:
  DS2Message message;
  ModelUpdaters updaters;
  std::shared_ptr<Model> model;
  quint64 counter{};
  int frequency{1};
};

#endif // REQUESTER_H
