import QtQuick 2.12
import com.tomicooler.e46track 1.0

ReplayForm {
    ReplayModel {
        id: replaymodel
        Component.onCompleted: {
            loadFile("/home/tomi/Downloads/2020-03-25 16:39:41.csv");
        }
    }

    startTimeLabel.text: new Date(replaymodel.model.timestamp).toLocaleString(Qt.locale(), "MM-dd hh:mm:ss")
    recordingSwitch.onClicked: {
        replaymodel.playPause();
    }
    dashboard.model: replaymodel.model
}
