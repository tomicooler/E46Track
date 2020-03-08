package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Clutch implements MessageHandler {
    private final Extractor extractor;
    private final TrackModel model;

    public Clutch(TrackModel model) {
        extractor = new Extractor(1, 1, false);
        this.model = model;
    }

    @Override
    public void process(Message message) {
        int flag = (int) extractor.extract(message);
        model.getCurrentClutch().postValue((flag & 0x11) == 0x11);
    }
}
