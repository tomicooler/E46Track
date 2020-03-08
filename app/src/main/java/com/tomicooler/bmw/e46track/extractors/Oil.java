package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Oil implements MessageHandler {

    private final Extractor extractor;
    private final TrackModel model;

    public Oil(TrackModel model) {
        extractor = new Extractor(12, 1, false, new Converter(0.75, -48.373));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        model.getCurrentOilTemp().postValue(extractor.extract(message));
    }
}
