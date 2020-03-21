package com.tomicooler.bmw.e46track;

import android.location.Location;

public class TrackModel {
    private long startTime;
    private long elapsedTime;

    private Location location;
    private double brake;
    private double throttle;
    private double steeringAngle;
    private double rpm;
    private double latG;
    private double yaw;

    public long getStartTime() {
        return startTime;
    }

    public void setStartTime(long startTime) {
        this.startTime = startTime;
    }

    public long getElapsedTime() {
        return elapsedTime;
    }

    public void setElapsedTime(long elapsedTime) {
        this.elapsedTime = elapsedTime;
    }

    public synchronized Location getLocation() {
        return location;
    }

    public synchronized void setLocation(Location location) {
        this.location = location;
    }

    public double getBrake() {
        return brake;
    }

    public void setBrake(double brake) {
        this.brake = brake;
    }

    public double getThrottle() {
        return throttle;
    }

    public void setThrottle(double throttle) {
        this.throttle = throttle;
    }

    public double getSteeringAngle() {
        return steeringAngle;
    }

    public void setSteeringAngle(double steeringAngle) {
        this.steeringAngle = steeringAngle;
    }

    public double getRpm() {
        return rpm;
    }

    public void setRpm(double rpm) {
        this.rpm = rpm;
    }

    public double getLatG() {
        return latG;
    }

    public void setLatG(double latG) {
        this.latG = latG;
    }

    public double getYaw() {
        return yaw;
    }

    public void setYaw(double yaw) {
        this.yaw = yaw;
    }
}
