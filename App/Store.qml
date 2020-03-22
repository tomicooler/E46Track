import QtQuick 2.14
import com.tomicooler.e46track 1.0

Item {
    property alias serial: serial
    property alias wifi: wifi
    property alias infoDialog: infoDialog
    property bool interfaceWifi: false

    InfoDialog {
        id: infoDialog
        anchors.fill: parent
    }

    Facade {
        id: facade

        onSendData: {
            if (interfaceWifi) {
                wifi.sendData(data);
            } else {
                serial.sendData(data);
            }
        }
    }

    SerialInterface {
        id: serial

        onConnected: {
            interfaceWifi = false;
            facade.connected();
        }

        onDataReceived: {
            facade.dataReceived(data);
        }

        onDisplayDialog: {
            infoDialog.displayDialog(title, details);
        }
    }

    TCPInterface {
        id: wifi

        onConnected: {
            interfaceWifi = true;
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
