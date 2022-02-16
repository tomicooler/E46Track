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

void VirtualControl::pressUp() { writeCommand("p_up"); }

void VirtualControl::pressDown() { writeCommand("p_down"); }

void VirtualControl::releaseUp() { writeCommand("r_up"); }

void VirtualControl::releaseDown() { writeCommand("r_down"); }

void VirtualControl::mouseMove(int x, int y) {
  writeCommand(QString{"m_move %1 %2"}.arg(x).arg(y).toUtf8());
}

void VirtualControl::readyRead() {
  qDebug() << qPrintable(m_process.readAll());
}

void VirtualControl::writeCommand(const QByteArray &command) {
  qDebug() << m_process.state() << command;
  m_process.write(command);
  m_process.write("\n");
  m_process.waitForBytesWritten();
}
