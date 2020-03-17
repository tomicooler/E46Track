import QtQuick 2.12

PedalForm {
    property double position: 0.0
    positionIndicator.height: position * height
}
