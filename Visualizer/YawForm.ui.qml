import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    id: wheel

    property alias label: label

    width: 80
    height: width
    radius: width / 2
    color: "transparent"
    border.width: width / 10
    border.color: "transparent"

    Rectangle {
        id: arrow
        width: wheel.border.width
        height: wheel.border.width
        radius: width / 2
        color: "black"
        antialiasing: true

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: qsTr("↑")
            anchors.centerIn: parent
            color: "white"
            font.bold: true
        }
    }

    Label {
        id: label
        anchors.horizontalCenter: arrow.horizontalCenter
        anchors.verticalCenter: arrow.verticalCenter
        anchors.verticalCenterOffset: arrow.height * 0.8
        color: "black"
        font.bold: true
        antialiasing: true
    }
}
