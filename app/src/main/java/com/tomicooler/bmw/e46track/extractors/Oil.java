package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Oil implements MessageHandler {

    private final Extractor extractor;
    private final TrackModel model;

    public Oil(TrackModel model) {
        extractor = new Extractor(12, 1,  new Converter(0.796098, -48.0137));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        model.getCurrentOilTemp().postValue(extractor.extract(message));
    }
}
