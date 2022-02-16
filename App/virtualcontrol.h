#ifndef VIRTUALCONTROL_H
#define VIRTUALCONTROL_H

#include <QObject>
#include <QProcess>

class VirtualControl : public QObject {
  Q_OBJECT
public:
  explicit VirtualControl(QObject *parent = nullptr);
  ~VirtualControl();

public slots:
  void pressUp();
  void releaseUp();
  void pressDown();
  void releaseDown();
  void mouseMove(int x, int y);

protected slots:
  void readyRead();

private:
  void writeCommand(const QByteArray &command);

private:
  QProcess m_process;
};

#endif // VIRTUALCONTROL_H
