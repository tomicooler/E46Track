import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    property alias yaw: yaw
    property alias steeringWheel: steeringWheel
    property alias brake: brake
    property alias kmphLabel: kmphLabel
    property alias throttle: throttle
    property alias rpm: rpm
    property alias latg: latg

    width: 400
    height: width * 1.1
    color: "gray"
    radius: width * 0.5

    Rectangle {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        color: parent.color
        width: parent.width * 0.5
        height: parent.height * 0.5
    }

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        color: parent.color
        width: parent.width * 0.5
        height: parent.height * 0.5
    }

    Rectangle {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: parent.color
        width: parent.width * 0.5
        height: parent.height * 0.5
    }

    Yaw {
        id: yaw
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        width: steeringWheel.width * 1.6
        height: steeringWheel.height * 1.6
    }

    SteeringWheel {
        id: steeringWheel
        width: parent.width * 0.4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: width * 0.4
        antialiasing: true
        smooth: true
    }

    Pedal {
        id: brake
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 3 / 4
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.horizontalCenterOffset: -steeringWheel.width * 3 / 4
        height: steeringWheel.height
        width: height / 8
    }

    Label {
        id: kmphLabel
        text: qsTr("0")
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: brake.verticalCenter
        font.bold: true
        font.pointSize: parent.width * 0.06
    }

    Label {
        text: qsTr("km/h")
        anchors.horizontalCenter: kmphLabel.horizontalCenter
        anchors.verticalCenter: kmphLabel.verticalCenter
        anchors.verticalCenterOffset: kmphLabel.height * 1.2
        font.bold: true
        font.pointSize: kmphLabel.font.pointSize
    }

    Pedal {
        id: throttle
        anchors.verticalCenter: brake.verticalCenter
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.horizontalCenterOffset: steeringWheel.width * 3 / 4
        height: brake.height
        width: brake.width
    }

    Rpm {
        id: rpm
        width: steeringWheel.width * 2
        height: brake.width
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 1.5
    }

    LateralG {
        id: latg
        width: rpm.width
        height: rpm.height
        anchors.horizontalCenter: rpm.horizontalCenter
        anchors.verticalCenter: rpm.verticalCenter
        anchors.verticalCenterOffset: rpm.height * 1.5
    }
}
