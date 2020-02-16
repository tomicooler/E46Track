package com.tomicooler.bmw.e46track;

import android.location.Location;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

class TrackModel extends ViewModel {

    private MutableLiveData<Location> location;

    MutableLiveData<Location> getCurrentLocation() {
        if (location == null) {
            location = new MutableLiveData<>();
        }
        return location;
    }
}