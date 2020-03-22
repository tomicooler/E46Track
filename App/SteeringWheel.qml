import QtQuick 2.12

SteeringWheelForm {
    property double angle
    rotation: angle
    label.text: qsTr("%1°").arg(rotation.toLocaleString(Qt.locale(), 'f', 0))
}
