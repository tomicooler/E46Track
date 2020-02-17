package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.Utils;
import com.tomicooler.bmw.e46track.ds2.Message;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

class Extractor {

    private final int bytepos;
    private final int length;
    private final boolean msb;
    private final Converter converter;

    Extractor(int bytepos, int length, boolean msb, final Converter converter) {
        this.bytepos = bytepos;
        this.length = length;
        this.msb = msb;
        this.converter = converter;
    }

    Extractor(int bytepos, int length, final Converter converter) {
        this.bytepos = bytepos;
        this.length = length;
        this.msb = false;
        this.converter = converter;
    }

    double extract(final Message message) {
        if (message.getData().length <= (bytepos + length)) {
            return -1.0;
        }

        ByteBuffer bb = ByteBuffer.wrap(Arrays.copyOfRange(message.getData(), bytepos, bytepos + length));
        if (length > 1) {
            bb.order(msb ? ByteOrder.BIG_ENDIAN : ByteOrder.LITTLE_ENDIAN);
        }

        double number = 0.0;

        switch (length) {
            case 1:
                number = bb.get() & 0xFF;
                break;

            case 2:
                number = bb.getShort();
                break;

            case 4:
                number = bb.getInt();
                break;
        }

        return converter.convert(number);
    }

}
