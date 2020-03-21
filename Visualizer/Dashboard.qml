import QtQuick 2.4
import QtQuick.Controls.Material 2.12

DashboardForm {
    property double kmph: 0
    kmphLabel.text: kmph | 0

    color: Material.color(Material.Grey, Material.Shade500)

    latg.smallIndicatorColor: Material.color(Material.Amber, Material.Shade200)
    latg.mediumIndicatorColor: Material.color(Material.Orange, Material.Shade200)
    latg.highIndicatorColor: Material.color(Material.DeepOrange, Material.Shade200)

    rpm.smallIndicatorColor: latg.smallIndicatorColor
    rpm.mediumIndicatorColor: latg.mediumIndicatorColor
    rpm.highIndicatorColor: latg.highIndicatorColor

    brake.indicatorColor: Material.color(Material.DeepOrange, Material.Shade800)
    throttle.indicatorColor: Material.color(Material.Green, Material.Shade800)

    steeringWheel.indicatorColor: brake.indicatorColor
}
