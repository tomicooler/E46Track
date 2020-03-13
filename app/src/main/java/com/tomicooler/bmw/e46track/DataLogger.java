package com.tomicooler.bmw.e46track;

import android.location.Location;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Locale;
import java.util.zip.GZIPOutputStream;

public class DataLogger implements Runnable {

    private final TrackModel model;
    private final String path;

    DataLogger(final TrackModel model, final String path) {
        this.model = model;
        this.path = path;
    }

    @Override
    public void run() {
        try {
            final long start = model.getCurrentStartTime().getValue();
            String fileName = String.format(Locale.getDefault(), "%s/e46track_%d_csv.gzip", path, start);
            System.out.println("saving to " + fileName);
            try (FileOutputStream output = new FileOutputStream(fileName);
                 Writer writer = new OutputStreamWriter(new GZIPOutputStream(output), "UTF-8");
               ) {
                writer.write("timestamp (utc milliseconds),latitude,longitude,altitude,bearing,speed (m/s),brake_pressure (bar),steering_angle (degree),throttle (Voltage),rpm,yaw (degree/sec),lat_G (g)\n");

                final char separator = ',';
                while (true) {
                    long now = System.currentTimeMillis();
                    model.getCurrentElapsedTime().postValue(now - start);

                    StringBuilder row = new StringBuilder();
                    final Location loc = model.getCurrentLocation().getValue();
                    row.append(now)
                            .append(separator)
                            .append(loc.getLatitude())
                            .append(separator)
                            .append(loc.getLongitude())
                            .append(separator)
                            .append(loc.getAltitude())
                            .append(separator)
                            .append(loc.getBearing())
                            .append(separator)
                            .append(loc.getSpeed())
                            .append(separator)
                            .append(model.getCurrentBrake().getValue())
                            .append(separator)
                            .append(model.getCurrentSteeringAngle().getValue())
                            .append(separator)
                            .append(model.getCurrentThrottle().getValue())
                            .append(separator)
                            .append(model.getCurrentRPM().getValue())
                            .append(separator)
                            .append(model.getCurrentYaw().getValue())
                            .append(separator)
                            .append(model.getCurrentLatG().getValue())
                            .append('\n');

                    writer.write(row.toString());
                    writer.flush();
                    Thread.sleep(80);
                }
            }
        } catch (InterruptedException | IOException e) {
            e.printStackTrace();
        }
    }

}
