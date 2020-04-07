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
    property alias position: position
    property alias size: size

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
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.fillWidth: true
            from: 0
            stepSize: 1
            hoverEnabled: true
            enabled: !exportButton.checked
        }

        RowLayout {
            Label {
                id: position
                Layout.leftMargin: 5
            }

            Item {
                Layout.fillWidth: true
            }

            Label {
                id: size
                Layout.rightMargin: 5
            }
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

            ToolButton {
                id: exportButton
                icon.name: qsTr("Export")
                icon.source: "qrc:/icons/get_app-black-48dp.svg"
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

            ToolButton {
                id: prevButton
                icon.name: qsTr("Previous")
                icon.source: "qrc:/icons/skip_previous-black-48dp.svg"
            }

            ToolButton {
                id: playPauseButton
                icon.name: checked ? qsTr("Pause") : qsTr("Play")
                icon.source: checked ? "qrc:/icons/pause-black-48dp.svg" : "qrc:/icons/play_arrow-black-48dp.svg"
                checkable: true
            }

            ToolButton {
                id: nextButton
                icon.name: qsTr("Next")
                icon.source: "qrc:/icons/skip_next-black-48dp.svg"
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
