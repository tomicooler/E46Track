import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import com.tomicooler.e46track 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 1020
    height: 720
    title: qsTr("E46Track Visualizer")

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 20
            Layout.margins: 10
            TextField {
                id: filepath
                Layout.fillWidth: true
                hoverEnabled: true
                text: "/home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584272470382_csv.gzip.csv"
                ToolTip.text: qsTr("First decompress the csv: 'cat e46track_1584264543426_csv.gzip | gzip -d &> input.csv'")
                selectByMouse: true
                ToolTip.visible: hovered
            }
            ToolButton {
                text: qsTr("Load")
                onClicked: {
                    csv.loadFile(filepath.text);
                }
            }
        }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 20
            Label {
                id: status
                visible: text.length > 0
            }
        }
    }

    GridLayout {
        anchors.fill: parent
        columns: 3

        Slider {
            id: slider
            Layout.columnSpan: 3
            Layout.fillWidth: true
            from: 0
        }

        ListView {
            id: list
            Layout.columnSpan: 3
            Layout.fillWidth: true
            model: csv.listModel
            orientation: ListView.Horizontal
            delegate: Label {
                text: model.modelData.timestamp
            }
        }

        Button {
            text: "grab"
            onClicked: {
                content.grabToImage(function(result) {
                    result.saveToFile("/tmp/something.png");
                });
            }
        }

        Button {
            text: "next"
            onClicked: csv.next()
        }
    }


    Item {
        id: content
        anchors.fill: parent

        Dashboard {
            id: dashboard
            width: parent.width * 0.2
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            Converter {
                id: converterBrake
                converted: csv.model.brake
                Component.onCompleted: {
                    set(-0.07, 105.0, 0.0, 1.0)
                }
            }

            Converter {
                id: converterThrottle
                converted: csv.model.throttle
                Component.onCompleted: {
                    set(0.75, 3.96, 0.0, 1.0)
                }
            }

            yaw.yaw: -1 * csv.model.yaw
            steeringWheel.angle: -1 * csv.model.steeringAngle
            brake.position: converterBrake.converted
            throttle.position: converterThrottle.converted
            kmph: csv.model.speed * 3.6
            rpm.rpm: csv.model.rpm
            latg.g: csv.model.latg
        }
    }

    CSVParser {
        id: csv
        onError: {
            status.text = message;
        }
        onStatus: {
            status.text = message;
        }
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
