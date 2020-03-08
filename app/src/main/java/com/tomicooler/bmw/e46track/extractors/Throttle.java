package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Throttle implements MessageHandler {

    private final Extractor extractor;
    private final TrackModel model;

    public Throttle(TrackModel model) {
        extractor = new Extractor(7, 1, false, new Converter(0.466406, 0.513281));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        model.getCurrentThrottle().postValue(extractor.extract(message));
    }
}
