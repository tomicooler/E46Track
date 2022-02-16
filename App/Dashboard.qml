import QtQuick 2.4
import QtQuick.Controls.Material 2.12
import QtQuick.Window 2.12
import com.tomicooler.e46track 1.0

DashboardForm {
    property Model model
    property bool kmphDisabled
    property int elapsed

    latg.smallIndicatorColor: Material.color(Material.Amber, Material.Shade200)
    latg.mediumIndicatorColor: Material.color(Material.Orange, Material.Shade200)
    latg.highIndicatorColor: Material.color(Material.DeepOrange, Material.Shade200)

    rpm.smallIndicatorColor: latg.smallIndicatorColor
    rpm.mediumIndicatorColor: latg.mediumIndicatorColor
    rpm.highIndicatorColor: latg.highIndicatorColor

    brake.indicatorColor: Material.color(Material.DeepOrange, Material.Shade800)
    throttle.indicatorColor: Material.color(Material.Green, Material.Shade800)

    steeringWheel.indicatorColor: brake.indicatorColor

    Converter {
        id: converterBrake
        converted: model.brake
        Component.onCompleted: {
            set(-0.07, 105.0, 0.0, 1.0)
        }
    }

    Converter {
        id: converterThrottle
        converted: model.throttle
        Component.onCompleted: {
            set(0.75, 3.94, 0.0, 1.0)
        }
    }

    Converter {
        id: converterSteeringAngle
        converted: model.steeringAngle > 0 ? Math.min(model.steeringAngle, 120) : Math.max(model.steeringAngle, -120)
        Component.onCompleted: {
            set(-120, 120, mouseMoveIndicator.from, mouseMoveIndicator.to)
        }
    }

    VirtualControl {
        id: control
    }

    mouseLeftIndicator.checked: converterThrottle.converted > 0.1
    mouseLeftIndicator.onCheckedChanged: mouseLeftIndicator.checked ? control.pressUp() : control.releaseUp()

    mouseRightIndicator.checked: converterBrake.converted > 0.1
    mouseRightIndicator.onCheckedChanged: mouseRightIndicator.checked ? control.pressDown() : control.releaseDown()

    mouseMoveIndicator.value: -1 * converterSteeringAngle.converted
    mouseMoveIndicator.onValueChanged: control.mouseMove((Screen.width / 2) + mouseMoveIndicator.value, Screen.height / 2)

    yaw.yaw: -1 * model.yaw
    steeringWheel.angle: -1 * model.steeringAngle
    brake.position: converterBrake.converted
    throttle.position: converterThrottle.converted
    kmphLabel.text: kmphDisabled ? new Date(elapsed).toLocaleString(Qt.locale(), "mm:ss.zzz") : qsTr("%1 km/h").arg(model.speed | 0)
    rpm.rpm: model.rpm
    latg.g: model.latg
}
