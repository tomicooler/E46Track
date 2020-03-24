#ifndef FACADE_H
#define FACADE_H

#include <QObject>

#include "datalogger.h"
#include "ds2message.h"
#include "requester.h"

class Facade : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT)
  Q_PROPERTY(DataLogger *logger READ logger CONSTANT)

public:
  explicit Facade(QObject *parent = nullptr);

  Model *model();
  DataLogger *logger();

signals:
  void sendData(const QByteArray &data);

public slots:
  void sendRequest();
  void dataReceived(const QByteArray &data);
  void connected();

private:
  std::shared_ptr<Model> m_model;
  std::vector<Requester> requesters;
  size_t index{};
  QByteArray buffer;
  DS2Parser parser;
  DataLogger m_logger;
};

#endif // FACADE_H
