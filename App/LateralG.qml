import QtQuick 2.12
import QtQml 2.12

LateralGForm {
    property double g: 0
    property color smallIndicatorColor: indicatorColor
    property color mediumIndicatorColor: indicatorColor
    property color highIndicatorColor: indicatorColor

    label.text: qsTr("%1 g").arg(Number(g).toLocaleString(Qt.locale(), 'f', 2))

    onGChanged: {
        if (g < 0) {
            minusMultiplier = 1.0 + g;
            plusMultiplier = 0.0;
        } else if (g > 0) {
            minusMultiplier = 1.0;
            plusMultiplier = Math.min(g, 1.0);
        } else {
            minusMultiplier = 1.0;
            plusMultiplier = 0.0;
        }

        var absG = Math.abs(g);
        if (absG <= 0.4) {
            indicatorColor = smallIndicatorColor;
        } else if (absG <= 0.7) {
            indicatorColor = mediumIndicatorColor;
        } else {
            indicatorColor = highIndicatorColor;
        }
    }
}
