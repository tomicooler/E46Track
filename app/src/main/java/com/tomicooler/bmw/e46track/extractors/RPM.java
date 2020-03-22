package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class RPM implements MessageHandler {

    private final Extractor extractorIntegral;
    private final Extractor extractorFractional;
    private final TrackModel model;

    public RPM(TrackModel model) {
        extractorIntegral = new Extractor(1, false, 1, true, new Converter(2.56, 0));
        extractorFractional = new Extractor(2, false, 1, false, new Converter(1.0/100.0, 0));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        final double integral = extractorIntegral.extract(message);
        final double fractional = extractorFractional.extract(message);
        model.setRpm((integral + fractional) * 100.0);
    }
}
