#ifndef VIRTUALCONTROL_H
#define VIRTUALCONTROL_H

#include <QObject>
#include <QProcess>

class VirtualControl : public QObject {
  Q_OBJECT

  Q_PROPERTY(int mouseX READ mouseX WRITE setMouseX NOTIFY mouseXChanged)
  Q_PROPERTY(int mouseY READ mouseY WRITE setMouseY NOTIFY mouseYChanged)

public:
  explicit VirtualControl(QObject *parent = nullptr);
  ~VirtualControl();

  Q_INVOKABLE void requestMousePos();

  int mouseX() const;
  void setMouseX(int newMouseX);
  int mouseY() const;
  void setMouseY(int newMouseY);

public slots:
  void pressUp();
  void releaseUp();
  void pressDown();
  void releaseDown();
  void mouseMove(int x, int y);

signals:
  void mouseXChanged();
  void mouseYChanged();

protected slots:
  void readyRead();

private:
  void writeCommand(const QByteArray &command);

private:
  QProcess m_process;
  int m_mouseX{0};
  int m_mouseY{0};
};

#endif // VIRTUALCONTROL_H
