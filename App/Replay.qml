import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import com.tomicooler.e46track 1.0

ReplayForm {
    property int frameIndex: 0;

    InfoDialog {
        id: info
        anchors.fill: parent
    }

    FileDialog {
        id: dialog
        title: qsTr("Please choose a CSV file")
        folder: replaymodel.directory()
        nameFilters: ["CSV files (*.csv)"]

        onAccepted: {
            replaymodel.loadUrl(dialog.fileUrl);
        }

        Component.onCompleted: {
            visible = true;
        }
    }

    ReplayModel {
        id: replaymodel
        property string exportPath: "/tmp"

        onModelChanged: {
            if (exportButton.checked) {
                dashboard.grabToImage(function(result) {
                    let filename = "%1.png".arg(String(frameIndex).padStart(10, '0'));
                    result.saveToFile(exportPath + "/" + filename);
                    replaymodel.currentFrameExported(filename);
                    frameIndex++;
                    replaymodel.next();
                });
            }
        }

        onIndexChanged: {
            if (index >= (size - 1)) {
                playPauseButton.checked = false;
            }

            slider.value = index
        }

        onError: info.displayDialog(qsTr("error"), message)
    }

    slider.to: replaymodel.size - 1
    slider.onValueChanged: replaymodel.index = slider.value
    position.text: replaymodel.index + 1
    size.text: replaymodel.size

    timestampLabel.text: new Date(replaymodel.model.timestamp).toLocaleString(Qt.locale(), "MM-dd hh:mm:ss")

    prevButton.onClicked: replaymodel.prev()
    prevButton.enabled: replaymodel.index >= 1 && !exportButton.checked
    playPauseButton.onClicked: replaymodel.playPause()
    playPauseButton.enabled: nextButton.enabled
    nextButton.onClicked: replaymodel.next()
    nextButton.enabled: replaymodel.index < (replaymodel.size - 1) && !exportButton.checked

    exportButton.onClicked: {
        replaymodel.exportPath = replaymodel.exportDirectory();
        replaymodel.writeFFmpegHelper();
        frameIndex = 0;
        replaymodel.index = replaymodel.index > 1 ? replaymodel.index - 1 : 0;
        replaymodel.next();
    }

    dashboard.kmphDisabled: !replaymodel.hasSpeed
    dashboard.kmphLabel.visible: !dashboard.kmphDisabled || !exportButton.checked
    dashboard.model: replaymodel.model
    dashboard.width: Number(dashWidth.text)
}
