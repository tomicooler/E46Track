#ifndef REPLAYMODEL_H
#define REPLAYMODEL_H

#include <QFile>
#include <QObject>
#include <QVector>

#include "model.h"

class ReplayModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT);
  Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged);
  Q_PROPERTY(int size READ size NOTIFY sizeChanged);

public:
  explicit ReplayModel(QObject *parent = nullptr);

  Q_INVOKABLE void loadFile(const QString &path);
  Q_INVOKABLE void next();
  Q_INVOKABLE void prev();
  Q_INVOKABLE void playPause();

  Model *model();
  int index() const;
  int size() const;

public slots:
  void setIndex(int index);

signals:
  void error(const QString &message);
  void status(const QString &message);
  void indexChanged(int index);
  void sizeChanged(int size);
  void modelChanged();

private:
  Model m_model;
  QVector<Model::Data> m_sequence;
  int m_index{};
  int m_size{};
  bool m_playing{false};
};

#endif // REPLAYMODEL_H
