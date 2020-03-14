package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Brake implements MessageHandler {

    private final Extractor extractorIntegral;
    private final Extractor extractorFractional;
    private final TrackModel model;

    public Brake(TrackModel model) {
        extractorIntegral = new Extractor(6, false, 1, true, new Converter(2.56, 0));
        extractorFractional = new Extractor(7, false, 1, false, new Converter(1.0/100.0, 0));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        model.getCurrentBrake().postValue(extractorIntegral.extract(message) + extractorFractional.extract(message));
    }
}