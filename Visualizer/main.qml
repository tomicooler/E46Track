import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Yaw {
        id: yaw
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        width: steeringWheel.width * 1.6
        height: steeringWheel.height * 1.6

        NumberAnimation on yaw {
            from: -60
            to: 60
            duration: 3000
        }
    }

    SteeringWheel {
        id: steeringWheel
        width: 160
        anchors.centerIn: parent

        NumberAnimation on rotation {
            to: 600
            duration: 3000
        }

        antialiasing: true
        smooth: true
    }

    Label {
        text: qsTr("%1°").arg(steeringWheel.rotation.toLocaleString(Qt.locale(), 'f', 0))
        anchors.centerIn: steeringWheel
        color: "white"
        font.bold: true
    }

    Pedal {
        id: brake
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 3 / 4
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.horizontalCenterOffset: -steeringWheel.width * 3 / 4
        height: steeringWheel.height
        width: height / 8

        NumberAnimation on position {
            to: 1.0
            duration: 3000
        }
    }

    Label {
        id: kmph
        text: steeringWheel.rotation.toLocaleString(Qt.locale(), 'f', 0)
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: brake.verticalCenter
        font.bold: true
        font.pointSize: 22
    }

    Label {
        text: qsTr("km/h")
        anchors.horizontalCenter: kmph.horizontalCenter
        anchors.verticalCenter: kmph.verticalCenter
        anchors.verticalCenterOffset: kmph.height * 1.2
        font.bold: true
        font.pointSize: 22
    }

    Pedal {
        id: throttle
        anchors.verticalCenter: brake.verticalCenter
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.horizontalCenterOffset: steeringWheel.width * 3 / 4
        height: brake.height
        width: brake.width
        indicatorColor: "green"

        NumberAnimation on position {
            to: 1.0
            duration: 3000
        }
    }

    Rpm {
        id: rpm
        width: steeringWheel.width * 2
        height: brake.width
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 1.5

        NumberAnimation on rpm {
            from: 0
            to: 8167
            duration: 3000
        }
    }

    LateralG {
        id: latg
        width: rpm.width
        height: rpm.height
        anchors.horizontalCenter: rpm.horizontalCenter
        anchors.verticalCenter: rpm.verticalCenter
        anchors.verticalCenterOffset: rpm.height * 1.5

        NumberAnimation on g {
            to: -1
            from: 1
            duration: 3000
        }
    }

    Component.onCompleted: {
        showMaximized();
    }
}
