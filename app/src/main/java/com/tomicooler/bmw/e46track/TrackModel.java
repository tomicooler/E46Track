package com.tomicooler.bmw.e46track;

import android.location.Location;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TrackModel extends ViewModel {

    private MutableLiveData<Location> location;
    private MutableLiveData<Double> oilTemp;
    private MutableLiveData<Boolean> clutch;
    private MutableLiveData<Boolean> brake;

    MutableLiveData<Location> getCurrentLocation() {
        if (location == null) {
            location = new MutableLiveData<>();
        }
        return location;
    }

    public MutableLiveData<Double> getCurrentOilTemp() {
        if (oilTemp == null) {
            oilTemp = new MutableLiveData<>();
        }
        return oilTemp;
    }

    public MutableLiveData<Boolean> getCurrentClutch() {
        if (clutch == null) {
            clutch = new MutableLiveData<>();
        }
        return clutch;
    }

    public MutableLiveData<Boolean> getCurrentBrake() {
        if (brake == null) {
            brake = new MutableLiveData<>();
        }
        return brake;
    }
}