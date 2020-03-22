import QtQuick 2.12
import QtQuick.Controls.Material 2.12

Rectangle {
    width: 15
    height: 25

    property alias positionIndicator: positionIndicator
    property color backgroundColor: "black"
    property color indicatorColor: "red"

    color: backgroundColor

    Rectangle {
        id: positionIndicator

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        width: parent.width
        height: 0
        color: indicatorColor
    }
}
