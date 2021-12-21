#ifndef REPLAYMODEL_H
#define REPLAYMODEL_H

#include <QFile>
#include <QObject>
#include <QUrl>
#include <QVector>

#include <optional>

#include "model.h"

class ReplayModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(Model *model READ model CONSTANT);
  Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged);
  Q_PROPERTY(int size READ size NOTIFY sizeChanged);
  Q_PROPERTY(bool hasSpeed READ hasSpeed NOTIFY hasSpeedChanged);

public:
  explicit ReplayModel(QObject *parent = nullptr);

  Q_INVOKABLE QUrl directory();
  Q_INVOKABLE QString exportDirectory();
  Q_INVOKABLE void loadUrl(const QUrl &url);
  Q_INVOKABLE void next();
  Q_INVOKABLE void prev();
  Q_INVOKABLE void playPause();
  Q_INVOKABLE void writeFFmpegHelper();
  Q_INVOKABLE void currentFrameExported(const QString &frameFilename);

  Model *model();
  int index() const;
  int size() const;
  bool hasSpeed() const;

public slots:
  void setIndex(int index);

signals:
  void error(const QString &message);
  void indexChanged(int index);
  void sizeChanged(int size);
  void modelChanged();
  void hasSpeedChanged(bool hasSpeed);

private:
  Model m_model;
  QVector<Model::Data> m_sequence;
  int m_index{};
  int m_size{};
  bool m_playing{false};
  QString m_exportDir;
  bool m_hasSpeed{false};
  std::optional<QFile> m_FFmpegConcat;
};

#endif // REPLAYMODEL_H
