package com.tomicooler.bmw.e46track;

public class DataLogger implements Runnable {

    final TrackModel model;

    DataLogger(final TrackModel model) {
        this.model = model;
    }

    @Override
    public void run() {
        try {
            while (true) {
                model.getCurrentElapsedTime().postValue(System.currentTimeMillis() - model.getCurrentStartTime().getValue());
                Thread.sleep(200);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

}
