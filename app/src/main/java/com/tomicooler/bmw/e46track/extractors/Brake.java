package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.TrackModel;
import com.tomicooler.bmw.e46track.ds2.Message;

public class Brake implements MessageHandler {

    private final Extractor extractorIntegral;
    private final Extractor extractorFractional;
    private final TrackModel model;

    public Brake(TrackModel model) {
        extractorIntegral = new Extractor(6, false, 1, false, new Converter(2.56, 0));
        extractorFractional = new Extractor(7, false, 1, false, new Converter(1.0/100.0, 0));
        this.model = model;
    }

    @Override
    public void process(Message message) {
        // request for offsets b829f102210241
        // byte pos 3 -> brake circuit 1 offset
        // byte pos 5 -> brake circuit 2 offset

        double integral = extractorIntegral.extract(message);

        if (integral > 200) {
            integral = 0.0;
        }
        double fractional = 0.0;
        if (integral > 0.0 ) {
            fractional = extractorFractional.extract(message);
        }

        model.getCurrentBrake().postValue(integral + fractional);
    }
}