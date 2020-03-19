import QtQuick 2.4

DashboardForm {
    property double kmph: 0
    kmphLabel.text: qsTr("%1°").arg(Number(kmph).toLocaleString(Qt.locale(), 'f', 0))
}
