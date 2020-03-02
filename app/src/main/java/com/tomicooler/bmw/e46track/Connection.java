package com.tomicooler.bmw.e46track;

import android.util.Log;

import com.tomicooler.bmw.e46track.ds2.Message;
import com.tomicooler.bmw.e46track.ds2.Parser;
import com.tomicooler.bmw.e46track.ds2.exceptions.InvalidChecksum;
import com.tomicooler.bmw.e46track.ds2.exceptions.NotEnoughData;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.util.List;

public class Connection implements Runnable {
    private static final String TAG = Connection.class.getSimpleName();

    private final InetAddress address;
    private final int port;
    private final List<Requester> requesters;
    private final TrackModel model;

    Connection(final InetAddress address, final int port, final List<Requester> requesters, final TrackModel model) {
        this.address = address;
        this.port = port;
        this.requesters = requesters;
        this.model = model;
    }

    @Override
    public void run() {
        try {
            Socket socket = new Socket(address, port);
            OutputStream out = socket.getOutputStream();
            InputStream in = socket.getInputStream();

            byte[] buffer = new byte[255];
            ByteArrayOutputStream inBuffer = new ByteArrayOutputStream();
            while (socket.isConnected()) {

                for (Requester requester : requesters) {
                    out.write(requester.getRequestMessageFramed());
                    out.flush();

                    boolean messageReceived = false;
                    while (!messageReceived) {
                        int size = in.read(buffer);
                        if (size == -1) {
                            break;
                        }
                        inBuffer.write(buffer, inBuffer.size(), size);
                        try {
                            Message message = Parser.parse(inBuffer.toByteArray());
                            requester.process(message);
                            inBuffer.reset();;
                            messageReceived = true;
                        } catch (NotEnoughData notEnoughData) {
                            Log.e(TAG, "Not enough data");
                        }
                    }

                    Thread.sleep(50);
                }

            }


        } catch (IOException e) {
            e.printStackTrace();
            Log.e(TAG, "Socket error", e);
            model.getCurrentError().postValue(e.getMessage());
        } catch (InvalidChecksum invalidChecksum) {
            invalidChecksum.printStackTrace();
            Log.e(TAG, "Invalid checksum");
            model.getCurrentError().postValue("Invalid checksum");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
}
