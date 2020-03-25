import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import com.tomicooler.e46track 1.0

ReplayForm {
    property int frameIndex: 0;

    FileDialog {
        id: dialog
        title: qsTr("Please coose a CSV file")
        folder: replaymodel.directory()
        nameFilters: ["CSV files (*.csv)"]

        onAccepted: {
            replaymodel.loadUrl(dialog.fileUrl)
            replaymodel.exportPath = replaymodel.exportDirectory();
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
                    result.saveToFile(exportPath + "/%1.png".arg(String(frameIndex).padStart(10, '0')));
                    frameIndex++;
                    replaymodel.next();
                });
            }
        }

        onIndexChanged: slider.value = index
    }

    slider.to: replaymodel.size
    slider.onValueChanged: replaymodel.index = slider.value

    timestampLabel.text: new Date(replaymodel.model.timestamp).toLocaleString(Qt.locale(), "MM-dd hh:mm:ss")

    prevButton.onClicked: replaymodel.prev()
    playPauseButton.onClicked: replaymodel.playPause()
    nextButton.onClicked: replaymodel.next()

    exportButton.onClicked: {
        frameIndex = 0;
        replaymodel.index = replaymodel.index > 1 ? replaymodel.index - 1 : 0;
        replaymodel.next();
    }

    dashboard.model: replaymodel.model
    dashboard.width: Number(dashWidth.text)
}
