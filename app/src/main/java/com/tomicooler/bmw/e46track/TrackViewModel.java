package com.tomicooler.bmw.e46track;

import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

public class TrackViewModel extends ViewModel {

    private MutableLiveData<String> error;
    private MutableLiveData<TrackModel > model;

    MutableLiveData<String> getCurrentError() {
        if (error == null) {
            error = new MutableLiveData<>();
        }
        return error;
    }

    MutableLiveData<TrackModel> getCurrentModel() {
        if (model == null) {
            model = new MutableLiveData<>();
        }
        return model;
    }
}