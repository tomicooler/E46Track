package com.tomicooler.bmw.e46track;

import android.location.Location;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TrackModel extends ViewModel {

    private MutableLiveData<String> error;
    private MutableLiveData<Long> startTime;
    private MutableLiveData<Long> elapsedTime;

    private MutableLiveData<Location> location;
    private MutableLiveData<Double> brake;
    private MutableLiveData<Double> throttle;
    private MutableLiveData<Double> steeringAngle;
    private MutableLiveData<Double> rpm;

    MutableLiveData<String> getCurrentError() {
        if (error == null) {
            error = new MutableLiveData<>();
        }
        return error;
    }

    MutableLiveData<Long> getCurrentStartTime() {
        if (startTime == null) {
            startTime = new MutableLiveData<>();
        }
        return startTime;
    }

    MutableLiveData<Long> getCurrentElapsedTime() {
        if (elapsedTime == null) {
            elapsedTime = new MutableLiveData<>();
        }
        return elapsedTime;
    }

    MutableLiveData<Location> getCurrentLocation() {
        if (location == null) {
            location = new MutableLiveData<>();
        }
        return location;
    }

    public MutableLiveData<Double> getCurrentBrake() {
        if (brake == null) {
            brake = new MutableLiveData<>();
        }
        return brake;
    }

    public MutableLiveData<Double> getCurrentThrottle() {
        if (throttle == null) {
            throttle = new MutableLiveData<>();
        }
        return throttle;
    }

    public MutableLiveData<Double> getCurrentSteeringAngle() {
        if (steeringAngle == null) {
            steeringAngle = new MutableLiveData<>();
        }
        return steeringAngle;
    }

    public MutableLiveData<Double> getCurrentRPM() {
        if (rpm == null) {
            rpm = new MutableLiveData<>();
        }
        return rpm;
    }
}