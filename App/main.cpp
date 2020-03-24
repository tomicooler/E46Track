#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifndef Q_OS_ANDROID
#include "serialinterface.h"
#endif
#include "tcpinterface.h"

#include "converter.h"
#include "facade.h"
#include "model.h"

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

#ifndef Q_OS_ANDROID
  qmlRegisterType<SerialInterface>("com.tomicooler.e46track", 1, 0,
                                   "SerialInterface");
#endif
  qmlRegisterType<TCPInterface>("com.tomicooler.e46track", 1, 0,
                                "TCPInterface");

  qmlRegisterType<Facade>("com.tomicooler.e46track", 1, 0, "Facade");
  qmlRegisterType<Model>("com.tomicooler.e46track", 1, 0, "Model");
  qmlRegisterType<Converter>("com.tomicooler.e46track", 1, 0, "Converter");

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}