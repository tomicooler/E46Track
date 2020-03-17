import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

Window {
    id: root
    visible: true
    width: 1920
    height: 1080
    title: qsTr("Hello World")

    Item {
        id: content
        anchors.fill: parent

        Dashboard {
            id: dashboard
            width: parent.width * 0.2
            anchors.bottom: parent.bottom
            anchors.right: parent.right

            NumberAnimation on yaw.yaw {
                from: -60
                to: 60
                duration: 3000
            }

            NumberAnimation on steeringWheel.angle {
                to: 600
                duration: 3000
            }

            NumberAnimation on brake.position {
                to: 1.0
                duration: 3000
            }

            NumberAnimation on throttle.position {
                to: 1.0
                duration: 3000
            }

            NumberAnimation on kmph {
                to: 250
                duration: 3000
            }

            NumberAnimation on rpm.rpm {
                from: 0
                to: 8167
                duration: 3000
            }

            NumberAnimation on latg.g {
                from: -1
                to: 1
                duration: 3000
            }
        }
    }


    Button {
        anchors.centerIn: parent
        text: "grab"
        onClicked: {
            content.grabToImage(function(result) {
                result.saveToFile("/tmp/something.png");
            });
        }
    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
