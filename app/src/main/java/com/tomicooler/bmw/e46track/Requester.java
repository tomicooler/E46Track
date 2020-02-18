package com.tomicooler.bmw.e46track;

import com.tomicooler.bmw.e46track.ds2.Message;
import com.tomicooler.bmw.e46track.extractors.MessageHandler;

import java.util.List;

class Requester {

    private byte[] requestMessage;
    private List<MessageHandler> handlers;

    Requester(final Message message, final List<MessageHandler> handlers) {
        requestMessage = message.serialize();
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
}
