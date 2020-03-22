import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: window
    visible: true
    width: 1080 / 2.5
    height: 2160 / 2.5
    title: qsTr("E46Track")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            icon.name: stackView.depth > 1 ? qsTr("Back") : qsTr("Settings")
            icon.source: stackView.depth > 1 ? "qrc:/icons/arrow-left.svg" : "qrc:/icons/bars.svg"
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            id: title
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.4
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Configuration")
                width: parent.width
                onClicked: {
                    //stackView.push(config)
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("SerialPort")
                width: parent.width
                onClicked: {
                    //stackView.push(serialport)
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("WiFi")
                width: parent.width
                onClicked: {
                    //stackView.push(wifipage)
                    drawer.close()
                }
            }
        }
    }

    Component {
        id: dashboard
        Dashboard {
            width: stackView.width
            height: stackView.height
        }
    }

    StackView {
        id: stackView
        initialItem: dashboard
        anchors.fill: parent
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
