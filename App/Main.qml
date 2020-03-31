import QtQuick 2.4
import com.tomicooler.e46track 1.0

MainForm {
    property Model model
    property DataLogger logger
    property int latencyMs

    startTimeLabel.text: new Date(logger.startTime).toLocaleString(Qt.locale(), "MM-dd hh:mm:ss")
    elapsedTimeLabel.text: new Date(logger.elapsedTime).toLocaleString(Qt.locale(), "mm:ss")
    latencyLabel.text: qsTr("Latency: %1 ms").arg(latencyMs)
    recordingSwitch.checked: logger.logging
    recordingSwitch.onClicked: {
        logger.logging = !logger.logging;
    }
    dashboard.model: model
}
