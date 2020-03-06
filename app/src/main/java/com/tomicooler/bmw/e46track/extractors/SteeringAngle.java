package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class SteeringAngle implements MessageHandler {

    private final Extractor extractor;
    private final TrackModel model;

    public SteeringAngle(TrackModel model) {
        extractor = new Extractor(4, 2, false, new Converter(0.045, 0.0));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        model.getCurrentSteeringAngle().postValue(extractor.extract(message));
    }
}
