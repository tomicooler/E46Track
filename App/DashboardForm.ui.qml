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
    property alias mouseLeftIndicator: mouseLeftIndicator
    property alias mouseRightIndicator: mouseRightIndicator
    property alias mouseMoveIndicator: mouseMoveIndicator
    property int minSize: width > height ? height : width
    color: "transparent"

    Column {
        anchors.top: parent.top
        anchors.right: parent.right
        Row {
            Button {
                id: mouseLeftIndicator
                text: qsTr("▲")
                checkable: true
            }
            Button {
                id: mouseRightIndicator
                text: qsTr("▼")
                checkable: true
            }
        }
        Slider {
            id: mouseMoveIndicator
            live: false
            stepSize: 1
            value: 0
            from: -850
            to: 850
        }
    }

    Yaw {
        id: yaw
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        width: steeringWheel.width * 1.8
        height: steeringWheel.height * 1.8
    }

    SteeringWheel {
        id: steeringWheel
        width: minSize * 0.4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -width * 0.25
        anchors.topMargin: width * 0.4
        antialiasing: true
        smooth: true
    }

    Pedal {
        id: brake
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 2 / 5
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.horizontalCenterOffset: -steeringWheel.width * 3 / 4
        height: steeringWheel.height
        width: height / 8
    }

    Label {
        id: kmphLabel
        text: qsTr("0")
        anchors.horizontalCenter: steeringWheel.horizontalCenter
        anchors.verticalCenter: steeringWheel.verticalCenter
        anchors.verticalCenterOffset: steeringWheel.height * 0.75
        font.bold: true
        font.pointSize: (minSize / 30) > 4 ? (minSize / 30) : 4
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
        anchors.verticalCenterOffset: steeringWheel.height * 1.1
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
