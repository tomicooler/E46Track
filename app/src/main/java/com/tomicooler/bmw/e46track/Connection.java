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

    Connection(final InetAddress address, final int port, final List<Requester> requesters) {
        this.address = address;
        this.port = port;
        this.requesters = requesters;
    }

    @Override
    public void run() {
        try {
            Socket socket = new Socket(address, port);
            OutputStream out = socket.getOutputStream();
            InputStream in = socket.getInputStream();

            while (socket.isConnected()) {

                for (Requester requester : requesters) {
                    out.write(Utils.hexStringToByteArray("000212c02100003c0005")); // extra framing
                    out.write(requester.getRequestMessage());
                    out.write(Utils.hexStringToByteArray("7a")); // extra framing

                    boolean messageReceived = false;
                    ByteArrayOutputStream inBuffer = new ByteArrayOutputStream();
                    while (!messageReceived) {
                        byte[] buffer = new byte[255];
                        int size = in.read(buffer);
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
        } catch (InvalidChecksum invalidChecksum) {
            invalidChecksum.printStackTrace();
            Log.e(TAG, "Invalid checksum");
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
}
