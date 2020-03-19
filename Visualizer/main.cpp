#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "csvparser.h"
#include "converter.h"
#include "model.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  qmlRegisterType<CSVParser>("com.tomicooler.e46track", 1, 0, "CSVParser");
  qmlRegisterType<Converter>("com.tomicooler.e46track", 1, 0, "Converter");
  qmlRegisterType<Model>("com.tomicooler.e46track", 1, 0, "Model");

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl) {
    if (!obj && url == objUrl)
      QCoreApplication::exit(-1);
  }, Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
