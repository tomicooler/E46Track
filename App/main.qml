import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 1080 / 2.5
    height: 2160 / 2.5
    title: qsTr("E46Track")

    header: ToolBar {

    }

    Component.onCompleted: {
        setX(Screen.width / 2 - width / 2);
        setY(Screen.height / 2 - height / 2);
    }
}
