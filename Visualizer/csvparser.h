#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QVariant>

#include "model.h"

class CSVParser : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Model *model READ model NOTIFY modelChanged);
  Q_PROPERTY(QVariant listModel READ listModel NOTIFY listModelChanged);

public:
  explicit CSVParser(QObject *parent = nullptr);

  Q_INVOKABLE void loadFile(const QString &path);
  Q_INVOKABLE bool next();

  Model * model() const;
  QVariant listModel() const;

signals:
  void error(const QString &message);
  void status(const QString &message);
  void listModelChanged(QList< Model* > listModel);
  void modelChanged(Model *model);

private:
  Model *m_model;
  QList< Model* > m_list;
  int index{};
};

#endif // CSVPARSER_H
