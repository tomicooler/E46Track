import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    property double multiplier: 0.0
    property color indicatorColor: "orange"
    property alias label: label

    width: 200
    height: 10
    border.width: 1
    border.color: "black"
    color: "white"

    Rectangle {
        id: plus
        width: (parent.width - parent.border.width) * multiplier
        height: parent.height - 2 * parent.border.width
        color: indicatorColor
        x: parent.border.width
        y: parent.border.width
    }

    Label {
        id: label
        font.bold: true
        font.pointSize: (parent.height / 2) > 4 ? (parent.height / 2) : 4
        anchors.centerIn: parent
    }
}
