package com.tomicooler.bmw.e46track.ds2;

public class Message {
    private final byte[] ecu;
    private final byte length;
    private final byte[] data;
    private byte checksum;

    public Message(byte []ecu, byte []data) {
        this.ecu = ecu;
        this.data = data;
        this.length = ecu.length == 1 ? (byte) (ecu.length + 1 + data.length + 1) : (byte) data.length;
        this.checksum = 0;
        for (byte b : ecu) {
            this.checksum ^= b;
        }
        this.checksum ^= length;
        for (byte b : data) {
            this.checksum ^= b;
        }
    }

    public byte[] getData() {
        return data;
    }

    public byte[] serialize() {
        byte []bytes = new byte[ecu.length + 1 + data.length + 1];
        System.arraycopy (ecu, 0, bytes, 0, ecu.length);
        bytes[ecu.length] = length;
        System.arraycopy (data, 0, bytes, ecu.length + 1, data.length);
        bytes[bytes.length - 1] = checksum;
        return bytes;
    }
}
