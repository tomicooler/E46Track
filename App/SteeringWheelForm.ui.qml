import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: wheel

    property color backgroundColor: "black"
    property color indicatorColor: "red"
    property alias label: label

    width: 80
    height: width
    radius: width / 2
    color: "transparent"
    border.width: width / 10
    border.color: backgroundColor
    antialiasing: true

    Rectangle {
        width: wheel.width
        height: wheel.border.width
        color: backgroundColor
        antialiasing: true

        anchors.verticalCenter: wheel.verticalCenter
    }

    Rectangle {
        width: wheel.border.width
        height: wheel.height / 2
        color: backgroundColor
        antialiasing: true

        anchors.horizontalCenter: wheel.horizontalCenter
        anchors.bottom: wheel.bottom
    }

    Rectangle {
        width: wheel.width / 3
        height: wheel.height / 3
        color: backgroundColor
        antialiasing: true

        radius: width / 2

        anchors.centerIn: parent
    }

    Rectangle {
        width: wheel.border.width
        height: wheel.border.width
        color: indicatorColor
        antialiasing: true

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Label {
        id: label
        anchors.centerIn: wheel
        color: "white"
        font.bold: true
        font.pointSize: (wheel.width / 14) > 4 ? (wheel.width / 14) : 4
        rotation: 0 - parent.rotation
    }
}
