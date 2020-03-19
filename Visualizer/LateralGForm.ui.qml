import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    property double minusMultiplier: 1.0
    property double plusMultiplier: 0.2
    property color indicatorColor: "orange"
    property alias label: label

    width: 200
    height: 10
    border.width: 1
    border.color: "black"
    color: "white"

    Rectangle {
        width: parent.width / 2 - parent.border.width
        height: parent.height - 2 * parent.border.width
        color: indicatorColor
        x: parent.border.width
        y: parent.border.width
    }

    Rectangle {
        id: minus
        width: ((parent.width / 2) - parent.border.width) * minusMultiplier
        height: parent.height - 2 * parent.border.width
        color: parent.color
        x: parent.border.width
        y: parent.border.width
    }

    Rectangle {
        id: plus
        width: ((parent.width / 2) - parent.border.width) * plusMultiplier
        height: parent.height - 2 * parent.border.width
        color: indicatorColor
        x: parent.border.width + ((parent.width / 2) - parent.border.width)
        y: parent.border.width
    }

    Label {
        id: label
        anchors.centerIn: parent
        font.bold: true
    }
}
