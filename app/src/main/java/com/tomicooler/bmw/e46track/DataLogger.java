package com.tomicooler.bmw.e46track;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.text.DateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.zip.GZIPOutputStream;

public class DataLogger implements AutoCloseable {

    private final TrackModel model;
    private FileOutputStream output;
    private Writer writer;
    private final long start;
    private final char separator = ',';

    DataLogger(final TrackModel model, final String path) throws IOException {
        this.model = model;

        start = model.getStartTime();
        String fileName = String.format(Locale.getDefault(), "%s/e46track_%s_csv.gzip", path, DateFormat.getDateTimeInstance().format(new Date(start)));
        System.out.println("saving to " + fileName);

        output = new FileOutputStream(fileName);
        writer = new OutputStreamWriter(new GZIPOutputStream(output), "UTF-8");
        writer.write("timestamp (utc milliseconds),latitude,longitude,altitude,bearing,speed (m/s),brake_pressure (bar),steering_angle (degree),throttle (Voltage),rpm,yaw (degree/sec),lat_G (g)\n");
    }

    void log() throws IOException {
        long now = System.currentTimeMillis();
        model.setElapsedTime(now - start);

        String row = String.valueOf(now) +
                separator +
                model.getLocation().getLatitude() +
                separator +
                model.getLocation().getLongitude() +
                separator +
                model.getLocation().getAltitude() +
                separator +
                model.getLocation().getBearing() +
                separator +
                model.getLocation().getSpeed() +
                separator +
                model.getBrake() +
                separator +
                model.getSteeringAngle() +
                separator +
                model.getThrottle() +
                separator +
                model.getRpm() +
                separator +
                model.getYaw() +
                separator +
                model.getLatG() +
                '\n';

        writer.write(row);
        writer.flush();
    }

    @Override
    public void close() throws Exception {
        writer.close();
        output.close();
    }
}
