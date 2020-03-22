import QtQuick 2.14
import com.tomicooler.e46track 1.0
import ".."

Item {
    property alias wifi: wifi
    property alias infoDialog: infoDialog

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    TCPInterface {
        id: wifi

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }
}
