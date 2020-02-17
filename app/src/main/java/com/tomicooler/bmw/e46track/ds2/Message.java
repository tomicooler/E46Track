package com.tomicooler.bmw.e46track.ds2;

public class Message {
    private final byte ecu;
    private final byte length;
    private final byte[] data;
    private byte checksum;

    public Message(byte ecu, byte []data) {
        this.ecu = ecu;
        this.length = (byte) (3 + data.length);
        this.data = data;
        this.checksum = ecu;
        this.checksum ^= length;
        for (Byte b : data) {
            this.checksum ^= b;
        }
    }

    Message(byte ecu, byte length, byte []data, byte checksum) {
        this.ecu = ecu;
        this.length = length;
        this.data = data;
        this.checksum = checksum;
    }

    public byte getEcu() {
        return ecu;
    }

    public byte[] getData() {
        return data;
    }

    public byte[] serialize() {
        byte []bytes = new byte[length];
        bytes[0] = ecu;
        bytes[1] = length;
        System.arraycopy (data, 0, bytes, 2, data.length);
        bytes[length - 1] = checksum;
        return bytes;
    }
}
