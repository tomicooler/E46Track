package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Throttle implements MessageHandler {

    private final Extractor extractorIntegral;
    private final Extractor extractorFractional;
    private final Converter converter;
    private final TrackModel model;

    public Throttle(TrackModel model) {
        extractorIntegral = new Extractor(24, false, 1, false, new Converter(2.56, 0));
        extractorFractional = new Extractor(25, false, 1, false, new Converter(1.0/100.0, 0));
        converter = new Converter(0.295, 0.75);
        this.model = model;
    }

    @Override
    public void process(Message message) {
        final double integral = extractorIntegral.extract(message);
        final double fractional = extractorFractional.extract(message);
        model.getCurrentThrottle().postValue(converter.convert(integral + fractional));
    }
}
