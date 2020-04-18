#include "location.h"

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
    qDebug() << "starting location updates" << source->updateInterval()
             << source->sourceName() << source->error();
    source->startUpdates();
  } else {
    qDebug() << "no location service was available";
  }
}

bool Location::hasLocation() const {
  return source && source->error() == QGeoPositionInfoSource::NoError;
}

void Location::positionUpdated(const QGeoPositionInfo &pos) {
  qDebug() << "location update" << pos;
  model->setLatitude(pos.coordinate().latitude());
  model->setLongitude(pos.coordinate().longitude());
  model->setAltitude(pos.coordinate().altitude());
  model->setBearing(pos.attribute(QGeoPositionInfo::Direction));
  model->setSpeed(pos.attribute(QGeoPositionInfo::GroundSpeed) * 3.6);
}
