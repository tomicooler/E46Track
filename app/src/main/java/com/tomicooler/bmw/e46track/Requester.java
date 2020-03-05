package com.tomicooler.bmw.e46track;

import com.tomicooler.bmw.e46track.ds2.Message;
import com.tomicooler.bmw.e46track.extractors.MessageHandler;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.List;

class Requester {

    private final byte[] requestMessage;
    private final byte[] requestMessageFramed;
    private List<MessageHandler> handlers;

    Requester(final Message message, final List<MessageHandler> handlers) throws IOException {
        requestMessage = message.serialize();
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        stream.write(Utils.hexStringToByteArray("000212c02100003c00"));
        stream.write((byte)requestMessage.length);
        stream.write(requestMessage);
        byte[] bytes = stream.toByteArray();
        byte checksum = 0;
        for (byte b : bytes) {
            checksum += b;
        }
        stream.write(checksum);
        requestMessageFramed = stream.toByteArray();

        this.handlers = handlers;
    }

    void process(final Message message) {
        if (handlers == null) {
            return;
        }

        for (MessageHandler handler : handlers) {
            handler.process(message);
        }
    }

    byte[] getRequestMessage() {
        return requestMessage;
    }

    byte[] getRequestMessageFramed() {
        return requestMessageFramed;
    }
}
