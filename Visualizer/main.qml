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

    property int exportFrameCount: 0

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 20
            Layout.margins: 10
            TextField {
                id: filepath
                Layout.fillWidth: true
                Layout.margins: 10
                hoverEnabled: true
                text: "/home/tomi/Desktop/pannoniaring2020_03_15/e46track/e46track_1584272470382_csv.gzip.csv"
                ToolTip.text: qsTr("First decompress the csv: 'cat e46track_1584264543426_csv.gzip | gzip -d &> input.csv'")
                selectByMouse: true
                ToolTip.visible: hovered
            }
            ToolButton {
                text: qsTr("Load")
                Layout.margins: 10
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
                Layout.margins: 10
                id: status
                visible: text.length > 0
            }
        }
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        Slider {
            id: slider
            Layout.margins: 20
            Layout.fillWidth: true
            from: 0
            to: csv.size - 1
            value: csv.index
            stepSize: 1
            hoverEnabled: true
            onValueChanged: {
                csv.index = value;
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            spacing: 20
            Button {
                text: qsTr("<")
                onClicked: csv.prev()
            }

            Button {
                text: checked ? qsTr("stop") : qsTr("play")
                onClicked: csv.playPause()
                checkable: true
            }

            Button {
                text: qsTr(">")
                onClicked: csv.next()
            }

            Item {
                Layout.fillWidth: true
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.fillWidth: true
            }

            TextField {
                id: videoWidth
                text: qsTr("1920")
                selectByMouse: true
            }

            TextField {
                id: videoHeight
                text: qsTr("1020")
                selectByMouse: true
            }

            Button {
                id: exportButton
                text: qsTr("Export")
                checkable: true
                onClicked: {
                    exportFrameCount = 0;
                    csv.index = 0;
                    csv.next();
                }
                ToolTip.text: qsTr("ffmpeg -framerate 60 -i /tmp/frame_%10d.png -c:v libx264 -pix_fmt yuv420p -crf 23 output.mp4")
                ToolTip.visible: hovered
                hoverEnabled: true
            }

            Item {
                Layout.fillWidth: true
            }
        }
    }

    Dashboard {
        id: dashboard
        width: videoWidth.text * 0.2
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

    CSVParser {
        id: csv
        onError: {
            status.text = message;
        }
        onStatus: {
            status.text = message;
        }
        onModelChanged: {
            status.text = new Date(model.timestamp);
            if (exportButton.checked) {
                dashboard.grabToImage(function(result) {
                    result.saveToFile("/tmp/frame_%1.png".arg(String(exportFrameCount).padStart(10, '0')));
                    exportFrameCount++;
                    csv.next();
                });
            }
        }
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
