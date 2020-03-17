import QtQuick 2.12

RpmForm {
    property double rpm: 0
    multiplier: Math.max(Math.min(8200.0, rpm), 0.0) / 8200.0
    label.text: rpm | 0
}
