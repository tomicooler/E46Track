import QtQuick 2.12
import QtQml 2.12

LateralGForm {
    property double g: 0

    label.text: qsTr("%1 g").arg(Number(g).toLocaleString(Qt.locale(), 'f', 2))

    onGChanged: {
        //var g = Math.min(Math.abs(acceleration / 9.81), 1.5) / 1.5 * (acceleration > 0 ? 1 : -1);
        if (g < 0) {
            minusMultiplier = 1.0 + g;
            plusMultiplier = 0.0;
        } else if (g > 0) {
            minusMultiplier = 1.0;
            plusMultiplier = g;
        } else {
            minusMultiplier = 1.0;
            plusMultiplier = 0.0;
        }

        var absG = Math.abs(g);
        if (absG <= 0.4) {
            indicatorColor = "green";
        } else if (absG <= 0.7) {
            indicatorColor = "orange";
        } else {
            indicatorColor = "red";
        }
    }
}
