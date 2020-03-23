import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import com.tomicooler.e46track 1.0

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
        Dashboard {
            width: stackView.width
            height: stackView.height

            Converter {
                id: converterBrake
                converted: store.model.brake
                Component.onCompleted: {
                    set(-0.07, 105.0, 0.0, 1.0)
                }
            }

            Converter {
                id: converterThrottle
                converted: store.model.throttle
                Component.onCompleted: {
                    set(0.75, 3.96, 0.0, 1.0)
                }
            }

            yaw.yaw: -1 * store.model.yaw
            steeringWheel.angle: -1 * store.model.steeringAngle
            brake.position: converterBrake.converted
            throttle.position: converterThrottle.converted
            kmph: store.model.speed * 3.6
            rpm.rpm: store.model.rpm
            latg.g: store.model.latg
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
