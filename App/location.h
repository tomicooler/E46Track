#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include <QGeoPositionInfoSource>

#include <model.h>
#include <memory>

class Location : public QObject {
  Q_OBJECT
public:
  explicit Location(const std::shared_ptr<Model> &model,
                    QObject *parent = nullptr);

  bool hasLocation() const;

protected slots:
  void positionUpdated(const QGeoPositionInfo &pos);

private:
  std::shared_ptr<Model> model;
  std::unique_ptr<QGeoPositionInfoSource> source;
};

#endif // LOCATION_H
