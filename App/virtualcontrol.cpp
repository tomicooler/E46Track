#include "virtualcontrol.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

VirtualControl::VirtualControl(QObject *parent) : QObject(parent) {
  QFileInfo info(QString("%1").arg(__FILE__));
  connect(&m_process, SIGNAL(readyRead()), this, SLOT(readyRead()));
  // First bootstrap virtualcontrol manually:
  //   ./gradlew install
  m_process.start(info.absoluteDir().path() +
                      "/virtualcontrol/app/build/install/app/bin/app",
                  QStringList{}, QIODevice::ReadWrite | QIODevice::Unbuffered);
  if (!m_process.waitForStarted()) {
    qDebug() << "ERROR" << m_process.errorString();
  }
}

VirtualControl::~VirtualControl() {
  m_process.kill();
  m_process.waitForFinished();
}

void VirtualControl::requestMousePos() { writeCommand("g_pos"); }

void VirtualControl::pressUp() { writeCommand("p_up"); }

void VirtualControl::pressDown() { writeCommand("p_down"); }

void VirtualControl::releaseUp() { writeCommand("r_up"); }

void VirtualControl::releaseDown() { writeCommand("r_down"); }

void VirtualControl::mouseMove(int x, int y) {
  writeCommand(QString{"m_move %1 %2"}.arg(x).arg(y).toUtf8());
}

void VirtualControl::readyRead() {
  while (m_process.canReadLine()) {
    QByteArray line = m_process.readLine();
    qDebug() << line;
    if (line.startsWith(QByteArray{"pos"})) {
      QList<QByteArray> split = line.split(' ');
      if (split.size() == 3) {
        setMouseX(split.at(1).toInt());
        setMouseY(split.at(2).toInt());
      }
    }
  }
}

void VirtualControl::writeCommand(const QByteArray &command) {
  qDebug() << m_process.state() << command;
  m_process.write(command);
  m_process.write("\n");
  m_process.waitForBytesWritten();
}

int VirtualControl::mouseX() const { return m_mouseX; }

void VirtualControl::setMouseX(int newMouseX) {
  if (m_mouseX == newMouseX)
    return;
  m_mouseX = newMouseX;
  emit mouseXChanged();
}

int VirtualControl::mouseY() const { return m_mouseY; }

void VirtualControl::setMouseY(int newMouseY) {
  if (m_mouseY == newMouseY)
    return;
  m_mouseY = newMouseY;
  emit mouseYChanged();
}
