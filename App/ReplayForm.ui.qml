import QtQuick 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Page {
    id: page

    property alias timestampLabel: timestampLabel
    property alias exportButton: exportButton
    property alias prevButton: prevButton
    property alias playPauseButton: playPauseButton
    property alias nextButton: nextButton
    property alias dashboard: dashboard
    property alias dashWidth: dashWidth
    property alias slider: slider

    width: 640
    height: 480
    title: qsTr("Replay")

    ColumnLayout {
        id: layout
        anchors.left: parent.left
        anchors.right: parent.right

        RowLayout {
            Layout.margins: 10
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Label {
                id: timestampLabel
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Slider {
            id: slider
            Layout.margins: 20
            Layout.fillWidth: true
            from: 0
            stepSize: 1
            hoverEnabled: true
            enabled: !exportButton.checked
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Size")
            }

            TextField {
                id: dashWidth
                Layout.maximumWidth: 40
                text: qsTr("400")
                selectByMouse: true
                inputMethodHints: Qt.ImhDigitsOnly
            }

            Label {
                text: qsTr("px")
            }

            Button {
                id: exportButton
                text: qsTr("Export")
                checkable: true
                enabled: !playPauseButton.checked
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Item {
                Layout.fillWidth: true
            }

            Button {
                id: prevButton
                text: qsTr("<")
            }

            Button {
                id: playPauseButton
                text: checked ? qsTr("stop") : qsTr("play")
                checkable: true
            }

            Button {
                id: nextButton
                text: qsTr(">")
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    Flickable {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: layout.bottom
        anchors.bottom: parent.bottom
        width: dashboard.width
        contentWidth: dashboard.width
        contentHeight: dashboard.height
        clip: true
        Dashboard {
            id: dashboard
            width: 400
            height: width
            color: Material.backgroundColor
        }
    }
}
