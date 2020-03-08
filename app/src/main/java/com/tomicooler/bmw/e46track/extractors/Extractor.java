package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.ds2.Message;

class Extractor {

    private final int bytepos;
    private final int length;
    private final boolean msb;
    private final Converter converter;
    private final boolean signed;

    Extractor(int bytepos, boolean msb, int length, boolean signed, final Converter converter) {
        this.bytepos = bytepos;
        this.length = length;
        this.msb = msb;
        this.converter = converter;
        this.signed = signed;
    }

    Extractor(int bytepos, int length, boolean signed, final Converter converter) {
        this.bytepos = bytepos;
        this.length = length;
        this.msb = false;
        this.converter = converter;
        this.signed = signed;
    }

    Extractor(int bytepos, int length, boolean signed) {
        this.bytepos = bytepos;
        this.length = length;
        this.msb = false;
        this.converter = null;
        this.signed = signed;
    }

    double extract(final Message message) {
        if (message.getData().length <= (bytepos + length)) {
            return -1.0;
        }

        double number;
        final byte[] bytes = message.getData();

        switch (length) {
            case 1:
                number = signed ? bytes[bytepos] : bytes[bytepos] & 0xFF;
                break;

            case 2: // getShort() is not OK. The data is not a 16 bit signed two's complement integer.
                int num;

                if (!msb) {
                    num = (bytes[bytepos] & 0xFF) + ((bytes[bytepos + 1] & 0xFF) << 8);
                } else {
                    num = (bytes[bytepos + 1] & 0xFF) + ((bytes[bytepos] & 0xFF) << 8);
                }

                if (signed) {
                    num = (num & 0x7FFF) * ((num & 0x8000) > 0 ? -1 : 1);
                }

                number = num;

                break;
            default:
                return -1.0;
        }

        return converter == null ? number : converter.convert(number);
    }

}
