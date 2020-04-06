import QtQuick 2.12
import com.tomicooler.e46track 1.0

ConfigurationForm {
    property Facade facade

    throttleAndRPMFrequency.onValueChanged: facade.setThrottleAndRPMFrequency(throttleAndRPMFrequency.value)
    speedFrequency.onValueChanged: facade.setSpeedFrequency(speedFrequency.value)
    dscOffsetsFrequency.onValueChanged: facade.setDscOffsetsFrequency(dscOffsetsFrequency.value)
    dscSteeringAngleFrequency.onValueChanged: facade.setDscSteeringAngleFrequency(dscSteeringAngleFrequency.value)
    dscBrakeYawLatgFrequency.onValueChanged: facade.setDscBrakeYawLatgFrequency(dscBrakeYawLatgFrequency.value)

    Component.onCompleted: {
        throttleAndRPMFrequency.value = facade.throttleAndRPMFrequency;
        speedFrequency.value = facade.speedFrequency;
        dscOffsetsFrequency.value = facade.dscOffsetsFrequency;
        dscSteeringAngleFrequency.value = facade.dscSteeringAngleFrequency;
        dscBrakeYawLatgFrequency.value = facade.dscBrakeYawLatgFrequency;
    }
}
