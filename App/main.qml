import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import com.tomicooler.e46track 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1020
    height: 720
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

    Store {
        id: store
        anchors.fill: parent
    }

    Drawer {
        id: drawer
        width: window.width * 0.4
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("SerialPort")
                width: parent.width
                visible: store.serialAvailable
                onClicked: {
                    stackView.push(serialport)
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("WiFi")
                width: parent.width
                onClicked: {
                    stackView.push(wifipage)
                    drawer.close()
                }
            }
        }
    }

    Component {
        id: dashboard
        Page {
            title: window.title

            ColumnLayout {
                anchors.fill: parent

                RowLayout {
                    Layout.margins: 10
                    spacing: 10

                    Item {
                        Layout.fillWidth: true
                    }

                    Label {
                        text: new Date(store.logger.startTime).toLocaleString(Qt.locale(), "MM-dd hh:mm:ss")
                    }

                    Label {
                        text: new Date(store.logger.elapsedTime).toLocaleString(Qt.locale(), "mm:ss")
                    }

                    Switch {
                        text: qsTr("Recording")
                        checked: store.logger.logging
                        onClicked: {
                            store.logger.logging = !store.logger.logging;
                        }
                    }

                    Item {
                        Layout.fillWidth: true
                    }
                }

                Dashboard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: store.model
                }
            }
        }
    }

    Component {
        id: serialport
        SerialPort {
            serial: store.serial
            width: stackView.width
            height: stackView.height
        }
    }

    Component {
        id: wifipage
        WiFiInterface {
            wifi: store.wifi
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
