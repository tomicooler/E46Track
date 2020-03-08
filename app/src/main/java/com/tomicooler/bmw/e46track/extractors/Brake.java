package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Brake implements MessageHandler {

    private final Extractor extractor;
    private final TrackModel model;

    public Brake(TrackModel model) {
        extractor = new Extractor(6, false, 1, false, new Converter(3.86947, -43.2321));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        double value = extractor.extract(message);
        if (value > 200) {
            value = 0.0;
        }
        model.getCurrentBrake().postValue(value);
    }
}