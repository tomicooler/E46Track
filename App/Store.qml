import QtQuick 2.14
import com.tomicooler.e46track 1.0

Item {
    property alias serial: serial
    property alias wifi: wifi
    property alias infoDialog: infoDialog

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    SerialInterface {
        id: serial

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }

    TCPInterface {
        id: wifi

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }
}
