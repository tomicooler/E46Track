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
                     const std::shared_ptr<Model> &model)
      : message(message), updaters(updaters), model(model) {}

  void processResponse(const DS2Message &response) {
    for (const auto &updater : updaters)
      updater->update(response, *model);
  }

  const DS2Message &get_message() { return message; }

private:
  DS2Message message;
  ModelUpdaters updaters;
  std::shared_ptr<Model> model;
};

#endif // REQUESTER_H
