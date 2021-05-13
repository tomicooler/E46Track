#include "location.h"

#include <QGuiApplication>
#include <QDebug>

Location::Location(const std::shared_ptr<Model> &model, QObject *parent)
    : QObject(parent), model(model),
      source(QGeoPositionInfoSource::createDefaultSource(nullptr)) {
  if (source) {
    connect(source.get(), &QGeoPositionInfoSource::positionUpdated, this,
            &Location::positionUpdated);
    source->setUpdateInterval(source->minimumUpdateInterval());
    source->setPreferredPositioningMethods(
        QGeoPositionInfoSource::AllPositioningMethods); // satelite as soon as
                                                        // available
    source->startUpdates();
    if (const auto *const gui =
            qobject_cast<const QGuiApplication *const>(qApp)) {
      connect(gui, &QGuiApplication::applicationStateChanged, this,
              [&](Qt::ApplicationState state) {
                switch (state) {
                case Qt::ApplicationState::ApplicationActive:
                  source->startUpdates();
                  break;
                default:
                  source->stopUpdates();
                }
              });
    }
  } else {
    qDebug() << "no location service was available";
  }
}

bool Location::hasLocation() const {
  return source && source->error() == QGeoPositionInfoSource::NoError;
}

void Location::positionUpdated(const QGeoPositionInfo &pos) {
  model->setLatitude(pos.coordinate().latitude());
  model->setLongitude(pos.coordinate().longitude());
  model->setAltitude(pos.coordinate().altitude());
  model->setBearing(pos.attribute(QGeoPositionInfo::Direction));
  model->setSpeed(pos.attribute(QGeoPositionInfo::GroundSpeed) * 3.6);
}
