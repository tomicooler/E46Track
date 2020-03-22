package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Yaw implements MessageHandler {

    private final Extractor extractorIntegral;
    private final Extractor extractorFractional;
    private final Converter converter1;
    private final Converter converter2;
    private final TrackModel model;

    public Yaw(TrackModel model) {
        extractorIntegral = new Extractor(10, false, 1, true, new Converter(2.56, 0));
        extractorFractional = new Extractor(11, false, 1, false, new Converter(1.0/100.0, 0));
        converter1 = new Converter(0.1, 0.0);
        converter2 = new Converter(2.85, 0.0);
        this.model = model;
    }

    @Override
    public void process(Message message) {
        final double integral = extractorIntegral.extract(message);
        final double fractional = extractorFractional.extract(message);
        model.setYaw(converter2.convert(converter1.convert(integral + fractional)));
    }
}