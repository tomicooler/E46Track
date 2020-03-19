import QtQuick 2.12

YawForm {
    property double yaw: 0.0
    rotation: yaw
    label.text: qsTr("%1 °/s").arg(Number(yaw).toLocaleString(Qt.locale(), 'f', 0))
}
