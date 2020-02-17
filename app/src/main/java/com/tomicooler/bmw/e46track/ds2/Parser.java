package com.tomicooler.bmw.e46track.ds2;

import com.tomicooler.bmw.e46track.ds2.exceptions.InvalidChecksum;
import com.tomicooler.bmw.e46track.ds2.exceptions.NotEnoughData;

import java.util.Arrays;


public class Parser {

    public static Message parse(final byte []data) throws NotEnoughData, InvalidChecksum {
        // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
        if (data.length < 3) {
            throw new NotEnoughData();
        }

        byte size = data[1];
        if (data.length < size) {
            throw new NotEnoughData();
        }

        byte checksum = 0;
        for (byte i = 0; i < size - 1; ++i) {
            checksum ^= data[i];
        }

        if (checksum != data[size - 1]) {
            throw new InvalidChecksum();
        }

        return new Message(data[0], data[1], Arrays.copyOfRange(data, 2, size - 3), checksum);
    }

}
