import QtQuick 2.12
import com.tomicooler.e46track 1.0

ReplayForm {
    property int frameIndex: 0;

    ReplayModel {
        id: replaymodel
        Component.onCompleted: {
            loadFile("/home/tomi/Downloads/2020-03-25 16:39:41.csv");
        }

        onModelChanged: {
            if (exportButton.checked) {
                dashboard.grabToImage(function(result) {
                    result.saveToFile("/tmp/frame_%1.png".arg(String(frameIndex).padStart(10, '0')));
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
