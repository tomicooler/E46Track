package com.tomicooler.bmw.e46track.extractors;

import com.tomicooler.bmw.e46track.ds2.Message;

public interface MessageHandler {
    void process(final Message message);
}
