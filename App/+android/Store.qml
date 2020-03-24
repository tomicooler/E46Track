import QtQuick 2.14
import com.tomicooler.e46track 1.0

Item {
    property alias wifi: wifi
    property alias infoDialog: infoDialog
    property alias model: facade.model
    property alias serial: serial
    property bool serialAvailable: false

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    Item {
        id: serial
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
