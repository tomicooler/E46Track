import QtQuick 2.14
import Qt.labs.settings 1.0
import com.tomicooler.e46track 1.0

Item {
    property alias wifi: wifi
    property alias infoDialog: infoDialog
    property alias model: facade.model
    property alias serial: serial
    property bool serialAvailable: false
    property alias logger: facade.logger
    property alias latencyMs: facade.latency
    property alias facade: facade

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

    Settings {
        property alias throttleAndRPMFrequency: facade.throttleAndRPMFrequency
        property alias speedFrequency: facade.speedFrequency
        property alias dscOffsetsFrequency: facade.dscOffsetsFrequency
        property alias dscSteeringAngleFrequency: facade.dscSteeringAngleFrequency
        property alias dscBrakeYawLatgFrequency: facade.dscBrakeYawLatgFrequency
    }

    TCPInterface {
        id: wifi

        onConnected: {
            facade.setDelay(0);
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
