import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Page {
    width: 400
    height: 400
    property alias dscBrakeYawLatgFrequency: dscBrakeYawLatgFrequency
    property alias dscSteeringAngleFrequency: dscSteeringAngleFrequency
    property alias dscOffsetsFrequency: dscOffsetsFrequency
    property alias speedFrequency: speedFrequency
    property alias throttleAndRPMFrequency: throttleAndRPMFrequency
    title: qsTr("Configuration")

    GridLayout {
        anchors.centerIn: parent
        columnSpacing: 5
        rowSpacing: 5
        columns: 2

        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("The lower the more frequent. Zero -> disable.")
            font.bold: true
        }

        Label {
            text: qsTr("Throttle & RPM")
        }

        Slider {
            id: throttleAndRPMFrequency
            from: 0
            to: 50
            stepSize: 1
        }

        Label {
            text: qsTr("Speed")
        }

        Slider {
            id: speedFrequency
            from: 0
            to: 50
            stepSize: 1
        }

        Label {
            text: qsTr("DSC offsets")
        }

        Slider {
            id: dscOffsetsFrequency
            from: 0
            to: 50
            stepSize: 1
        }

        Label {
            text: qsTr("Steering Andle")
        }

        Slider {
            id: dscSteeringAngleFrequency
            from: 0
            to: 50
            stepSize: 1
        }

        Label {
            text: qsTr("Brake & Yaw & LatG")
        }

        Slider {
            id: dscBrakeYawLatgFrequency
            from: 0
            to: 50
            stepSize: 1
        }
    }
}
