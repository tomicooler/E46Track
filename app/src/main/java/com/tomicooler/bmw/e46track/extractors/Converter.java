package com.tomicooler.bmw.e46track.extractors;

class Converter {
    private final double multiplier;
    private final double offset;

    Converter(final double multiplier, final double offset) {
        this.multiplier = multiplier;
        this.offset = offset;
    }

    double convert(double value) {
        return (value * multiplier) + offset;
    }
}
