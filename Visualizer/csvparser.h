#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <QObject>
#include <QVector>
#include <QFile>

#include "model.h"

class CSVParser : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Model *model READ model NOTIFY modelChanged);
  Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged);
  Q_PROPERTY(int size READ size NOTIFY sizeChanged);

public:
  explicit CSVParser(QObject *parent = nullptr);

  Q_INVOKABLE void loadFile(const QString &path);
  Q_INVOKABLE void next();
  Q_INVOKABLE void prev();
  Q_INVOKABLE void playPause();

  Model * model() const;
  int index() const;
  int size() const;

public slots:
  void setIndex(int index);

signals:
  void error(const QString &message);
  void status(const QString &message);
  void listModelChanged(QList< Model* > listModel);
  void modelChanged(Model *model);

  void indexChanged(int index);

  void sizeChanged(int size);

private:
  Model *m_model;
  QVector< Model::Data > m_sequence;
  int m_index{};
  int m_size{};
  bool m_playing{false};
};

#endif // CSVPARSER_H
