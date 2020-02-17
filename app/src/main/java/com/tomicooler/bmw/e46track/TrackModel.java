package com.tomicooler.bmw.e46track;

import android.location.Location;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TrackModel extends ViewModel {

    private MutableLiveData<Location> location;
    private MutableLiveData<Double> oilTemp;

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
}