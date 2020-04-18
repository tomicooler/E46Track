#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>

#ifndef Q_OS_ANDROID
#include "serialinterface.h"
#endif
#include "tcpinterface.h"

#include "converter.h"
#include "facade.h"
#include "model.h"
#include "replaymodel.h"

#if defined(Q_OS_ANDROID)
#include <QtAndroid>

bool requestAndroidPermissions() {
  const QVector<QString> permissions(
      {"android.permission.ACCESS_FINE_LOCATION",
       "android.permission.WRITE_EXTERNAL_STORAGE",
       "android.permission.READ_EXTERNAL_STORAGE"});

  for (const QString &permission : permissions) {
    auto result = QtAndroid::checkPermission(permission);
    if (result == QtAndroid::PermissionResult::Denied) {
      auto resultHash =
          QtAndroid::requestPermissionsSync(QStringList({permission}));
      if (resultHash[permission] == QtAndroid::PermissionResult::Denied)
        return false;
    }
  }

  return true;
}
#endif

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/icons/appicon.png"));

#ifndef Q_OS_ANDROID
  qmlRegisterType<SerialInterface>("com.tomicooler.e46track", 1, 0,
                                   "SerialInterface");
#else
  if (!requestAndroidPermissions())
    return -1;
#endif
  qmlRegisterType<TCPInterface>("com.tomicooler.e46track", 1, 0,
                                "TCPInterface");

  qmlRegisterType<Facade>("com.tomicooler.e46track", 1, 0, "Facade");
  qmlRegisterType<Model>("com.tomicooler.e46track", 1, 0, "Model");
  qmlRegisterType<Converter>("com.tomicooler.e46track", 1, 0, "Converter");
  qmlRegisterType<DataLogger>("com.tomicooler.e46track", 1, 0, "DataLogger");
  qmlRegisterType<ReplayModel>("com.tomicooler.e46track", 1, 0, "ReplayModel");

  app.setOrganizationName("E46Track");
  app.setOrganizationDomain("com.tomicooler.bmw");

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
