import QtQuick 2.12

RpmForm {
    property double rpm: 0
    property color smallIndicatorColor: indicatorColor
    property color mediumIndicatorColor: indicatorColor
    property color highIndicatorColor: indicatorColor

    multiplier: Math.max(Math.min(8200.0, rpm), 0.0) / 8200.0
    label.text: rpm | 0

    onRpmChanged: {
        if (rpm <= 4000) {
            indicatorColor = smallIndicatorColor;
        } else if (rpm <= 7500) {
            indicatorColor = mediumIndicatorColor;
        } else {
            indicatorColor = highIndicatorColor;
        }
    }
}
