package com.tomicooler.bmw.e46track;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.content.res.Configuration;
import android.location.Location;
import android.os.Binder;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.core.app.NotificationCompat;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.tomicooler.bmw.e46track.ds2.Message;
import com.tomicooler.bmw.e46track.extractors.ClutchBrake;
import com.tomicooler.bmw.e46track.extractors.MessageHandler;
import com.tomicooler.bmw.e46track.extractors.Oil;
import com.tomicooler.bmw.e46track.extractors.SteeringAngle;
import com.tomicooler.bmw.e46track.extractors.Throttle;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class TrackService extends Service {
    private static final String TAG = TrackService.class.getSimpleName();

    private static final String CHANNEL_ID = "channel_01";
    private static final int NOTIFICATION_ID = 12345678;

    private static final long UPDATE_INTERVAL_IN_MILLISECONDS = 500;
    private static final long FASTEST_UPDATE_INTERVAL_IN_MILLISECONDS = UPDATE_INTERVAL_IN_MILLISECONDS / 2;
    private LocationRequest mLocationRequest;
    private FusedLocationProviderClient mFusedLocationClient;
    private LocationCallback mLocationCallback;

    private boolean mChangingConfiguration = false;
    private final IBinder mBinder = new LocalBinder();
    private Handler mServiceHandler;
    private TrackModel model;

    private Future connectionFuture;

    private boolean mIsTracking;

    public TrackService() {
        mIsTracking = false;
    }

    @Override
    public void onCreate() {
        model = new TrackModel();

        mFusedLocationClient = LocationServices.getFusedLocationProviderClient(this);

        mLocationCallback = new LocationCallback() {
            @Override
            public void onLocationResult(LocationResult locationResult) {
                super.onLocationResult(locationResult);
                onNewLocation(locationResult.getLastLocation());
            }
        };

        createLocationRequest();
        getLastLocation();

        HandlerThread handlerThread = new HandlerThread(TAG);
        handlerThread.start();
        mServiceHandler = new Handler(handlerThread.getLooper());
        NotificationManager mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        CharSequence name = getString(R.string.app_name);

        assert mNotificationManager != null;
        NotificationChannel mChannel = new NotificationChannel(CHANNEL_ID, name, NotificationManager.IMPORTANCE_DEFAULT);
        mNotificationManager.createNotificationChannel(mChannel);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "Service started");
        return START_NOT_STICKY;
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        mChangingConfiguration = true;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.i(TAG, "in onBind()");
        stopForeground(true);
        mChangingConfiguration = false;
        return mBinder;
    }

    @Override
    public void onRebind(Intent intent) {
        Log.i(TAG, "in onRebind()");
        stopForeground(true);
        mChangingConfiguration = false;
        super.onRebind(intent);
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.i(TAG, "Last client unbound from service");

        if (!mChangingConfiguration && isTracking()) {
            Log.i(TAG, "Starting foreground service");
            startForeground(NOTIFICATION_ID, getNotification());
        }
        return true;
    }

    @Override
    public void onDestroy() {
        mServiceHandler.removeCallbacksAndMessages(null);
    }

    public static List<MessageHandler> messageHandlers(final MessageHandler handler) {
        List<MessageHandler> handlers = new ArrayList<>();
        handlers.add(handler);
        return handlers;
    }

    public void starTracking() {
        Log.i(TAG, "Requesting location updates");
        startService(new Intent(getApplicationContext(), TrackService.class));
        mIsTracking = true;
        try {
            mFusedLocationClient.requestLocationUpdates(mLocationRequest, mLocationCallback, Looper.myLooper());
            ExecutorService executorService = Executors.newSingleThreadExecutor();

            List<Requester> requesters = new ArrayList<>();

            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("12")[0], Utils.hexStringToByteArray("0b03")), messageHandlers(new Oil(model))));
            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("12")[0], Utils.hexStringToByteArray("0b04")), messageHandlers(new ClutchBrake(model))));
            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("12")[0], Utils.hexStringToByteArray("0b92")), messageHandlers(new Throttle(model))));

            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("57")[0], Utils.hexStringToByteArray("00")), null));
            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("57")[0], Utils.hexStringToByteArray("1b01")), messageHandlers(new SteeringAngle(model))));
            requesters.add(new Requester(new Message(Utils.hexStringToByteArray("57")[0], Utils.hexStringToByteArray("00")), null));

            Connection connection = new Connection(InetAddress.getByName(Utils.address(getApplicationContext())), Utils.port(getApplicationContext()), requesters);
            connectionFuture = executorService.submit(connection);
        } catch (SecurityException unlikely) {
            mIsTracking = false;
            Log.e(TAG, "Lost location permission. Could not request updates. " + unlikely);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    public void stopTracking() {
        Log.i(TAG, "Removing location updates");
        try {
            mFusedLocationClient.removeLocationUpdates(mLocationCallback);
            if (connectionFuture != null) {
                connectionFuture.cancel(true);
            }
            mIsTracking = false;
            stopSelf();
        } catch (SecurityException unlikely) {
            mIsTracking = true;
            Log.e(TAG, "Lost location permission. Could not remove updates. " + unlikely);
        }
    }

    public TrackModel getModel() {
        return model;
    }

    private Notification getNotification() {
        PendingIntent activityPendingIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, MainActivity.class), 0);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setContentIntent(activityPendingIntent)
                .setContentText(getString(R.string.notification_content))
                .setSmallIcon(R.mipmap.ic_launcher);


        return builder.build();
    }

    private void getLastLocation() {
        try {
            mFusedLocationClient.getLastLocation()
                    .addOnCompleteListener(new OnCompleteListener<Location>() {
                        @Override
                        public void onComplete(@NonNull Task<Location> task) {
                            if (task.isSuccessful() && task.getResult() != null) {
                                model.getCurrentLocation().setValue(task.getResult());
                            } else {
                                Log.w(TAG, "Failed to get location.");
                            }
                        }
                    });
        } catch (SecurityException unlikely) {
            Log.e(TAG, "Lost location permission." + unlikely);
        }
    }

    private void onNewLocation(Location location) {
        model.getCurrentLocation().setValue(location);
    }

    private void createLocationRequest() {
        mLocationRequest = new LocationRequest();
        mLocationRequest.setInterval(UPDATE_INTERVAL_IN_MILLISECONDS);
        mLocationRequest.setFastestInterval(FASTEST_UPDATE_INTERVAL_IN_MILLISECONDS);
        mLocationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
    }

    public boolean isTracking() {
        return mIsTracking;
    }

    class LocalBinder extends Binder {
        TrackService getService() {
            return TrackService.this;
        }
    }

}
