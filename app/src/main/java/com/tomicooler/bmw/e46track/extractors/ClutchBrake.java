package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class ClutchBrake implements MessageHandler {
    private final Extractor extractor;
    private final TrackModel model;

    public ClutchBrake(TrackModel model) {
        extractor = new Extractor(1, 1, false);
        this.model = model;
    }

    @Override
    public void process(Message message) {
        int flag = (int) extractor.extract(message);
        model.getCurrentBrake().postValue((flag & 0x0C) == 0x0C);
        model.getCurrentClutch().postValue((flag & 0x11) == 0x11);
    }
}
