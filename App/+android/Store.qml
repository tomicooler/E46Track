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

    Facade {
        id: facade

        onSendData: {
            wifi.sendData(data);
        }
    }

    TCPInterface {
        id: wifi

        onConnected: {
            facade.connected();
        }

        onDataReceived: {
            facade.dataReceived(data);
        }

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }
}
