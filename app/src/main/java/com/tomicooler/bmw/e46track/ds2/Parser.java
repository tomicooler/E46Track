package com.tomicooler.bmw.e46track.ds2;

import com.tomicooler.bmw.e46track.Utils;
import com.tomicooler.bmw.e46track.ds2.exceptions.InvalidChecksum;
import com.tomicooler.bmw.e46track.ds2.exceptions.NotEnoughData;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


public class Parser {

    private final List<byte[]> knownLongEcus;

    public Parser() {
        knownLongEcus = new ArrayList<>();
        knownLongEcus.add(Utils.hexStringToByteArray("b829f1"));
        knownLongEcus.add(Utils.hexStringToByteArray("b8f129"));
    }

    public Message parse(final byte []data) throws NotEnoughData, InvalidChecksum {
        byte ecuSize = 1;
        for (final byte[] ecu : knownLongEcus) {
            if (startsWith(ecu, data)) {
                ecuSize = (byte)ecu.length;
                break;
            }
        }

        // [ECU ADDRESS] [LENGTH] [DATA1] [DATAN] [CHECKSUM]
        if (data.length < ecuSize + 2) {
            throw new NotEnoughData();
        }

        int packetSize = data[ecuSize];
        int dataSize = packetSize - ecuSize - 1;

        if (ecuSize > 1) {
            dataSize = data[ecuSize];
            packetSize = ecuSize + dataSize + 2;
        }

        if (data.length < packetSize) {
            throw new NotEnoughData();
        }

        byte checksum = 0;
        for (byte i = 0; i < packetSize - 1; ++i) {
            checksum ^= data[i];
        }

        if (checksum != data[packetSize - 1]) {
            throw new InvalidChecksum();
        }

        return new Message(Arrays.copyOfRange(data, 0, ecuSize), Arrays.copyOfRange(data, ecuSize + 1, ecuSize + 1 + dataSize));
    }

    private static boolean startsWith(final byte[] match, final byte[] data) {
        if (data.length < match.length)
            return false;

        for (int i = 0; i < match.length; ++i) {
            if (match[i] != data[i])
                return false;
        }

        return true;
    }

}
