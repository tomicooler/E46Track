package com.tomicooler.bmw.e46track;

import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.app.ActivityCompat;
import androidx.lifecycle.Observer;

import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import android.Manifest;

import android.content.pm.PackageManager;

import android.net.Uri;

import android.provider.Settings;

import android.widget.TextView;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();

    private static final int REQUEST_PERMISSIONS_REQUEST_CODE = 34;

    private TrackService mService = null;
    private boolean mBound = false;

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            TrackService.LocalBinder binder = (TrackService.LocalBinder) service;
            mService = binder.getService();
            mBound = true;
            setFabIcon();

            final TextView latitude = findViewById(R.id.latitude);
            final TextView longitude = findViewById(R.id.longitude);
            final TextView altitude = findViewById(R.id.altitude);
            final TextView speed = findViewById(R.id.speed);
            final TextView bearing = findViewById(R.id.bearing);


            final TextView brake = findViewById(R.id.brake);
            final TextView throttle = findViewById(R.id.throttle);
            final TextView steeringAngle = findViewById(R.id.steeringAngle);
            final TextView rpm = findViewById(R.id.rpm);
            final TextView yaw = findViewById(R.id.yaw);
            final TextView latG = findViewById(R.id.latG);

            final TextView startTime = findViewById(R.id.startTime);
            final TextView elapsedTime = findViewById(R.id.elapsedTime);

            final DateFormat startTimeFormatter = DateFormat.getTimeInstance();
            final DateFormat elapsedFormatter = new SimpleDateFormat("mm:ss", Locale.getDefault());

            mService.getViewModel().getCurrentModel().observeForever(new Observer<TrackModel>() {
                @Override
                public void onChanged(TrackModel m) {
                    latitude.setText(String.format(Locale.getDefault(), "%f", m.getLocation().getLatitude()));
                    longitude.setText(String.format(Locale.getDefault(), "%f", m.getLocation().getLongitude()));
                    altitude.setText(String.format(Locale.getDefault(), "%f", m.getLocation().getAltitude()));
                    speed.setText(String.format(Locale.getDefault(), "%f", m.getLocation().getSpeed() * 3.6));
                    bearing.setText(String.format(Locale.getDefault(), "%f", m.getLocation().getBearing()));

                    brake.setText(String.format(Locale.getDefault(), "%f", m.getBrake()));
                    throttle.setText(String.format(Locale.getDefault(), "%f", m.getThrottle()));
                    steeringAngle.setText(String.format(Locale.getDefault(), "%f", m.getSteeringAngle()));
                    rpm.setText(String.format(Locale.getDefault(), "%f", m.getRpm()));
                    yaw.setText(String.format(Locale.getDefault(), "%f", m.getYaw()));
                    latG.setText(String.format(Locale.getDefault(), "%f", m.getLatG()));

                    startTime.setText(String.format(Locale.getDefault(), "%s", startTimeFormatter.format(new Date(m.getStartTime()))));
                    elapsedTime.setText(String.format(Locale.getDefault(), "%s", elapsedFormatter.format(new Date(m.getElapsedTime()))));
                }
            });

            final TextView error = findViewById(R.id.error);
            mService.getViewModel().getCurrentError().observeForever(new Observer<String>() {
                @Override
                public void onChanged(String value) {
                    error.setText(value);
                    if (mService != null && !value.isEmpty()) {
                        mService.stopTracking();
                        setFabIcon();
                    }
                }
            });
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mService = null;
            mBound = false;
        }
    };

    FloatingActionButton fab;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        fab = findViewById(R.id.fab);
        setFabIcon();
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!checkPermissions()) {
                    requestPermissions();
                } else {
                    if (mService.isTracking()) {
                        mService.stopTracking();
                        Snackbar.make(view, "Stopped...", Snackbar.LENGTH_LONG).show();
                    } else {
                        mService.starTracking();
                        Snackbar.make(view, "Started...", Snackbar.LENGTH_LONG).show();
                    }
                    setFabIcon();
                }
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.action_settings) {
            Intent intent = new Intent(this, SettingsActivity.class);
            startActivity(intent);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onStart() {
        super.onStart();

        bindService(new Intent(this, TrackService.class), mServiceConnection, Context.BIND_AUTO_CREATE);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStop() {
        if (mBound) {
            unbindService(mServiceConnection);
            mBound = false;
        }
        super.onStop();
    }

    private void setFabIcon() {
        fab.setImageResource(mService != null && mService.isTracking() ? android.R.drawable.ic_media_pause : android.R.drawable.ic_media_play);
    }

    private boolean checkPermissions() {
        return PackageManager.PERMISSION_GRANTED == ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION);
    }

    private void requestPermissions() {
        boolean shouldProvideRationale =
                ActivityCompat.shouldShowRequestPermissionRationale(this,
                        Manifest.permission.ACCESS_FINE_LOCATION);

        if (shouldProvideRationale) {
            Log.i(TAG, "Displaying permission rationale to provide additional context.");
            Snackbar.make(
                    findViewById(R.id.activity_main),
                    R.string.permission_rationale,
                    Snackbar.LENGTH_INDEFINITE)
                    .setAction(R.string.ok, new View.OnClickListener() {
                        @Override
                        public void onClick(View view) {
                            ActivityCompat.requestPermissions(MainActivity.this,
                                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                                    REQUEST_PERMISSIONS_REQUEST_CODE);
                        }
                    })
                    .show();
        } else {
            Log.i(TAG, "Requesting permission");
            ActivityCompat.requestPermissions(MainActivity.this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    REQUEST_PERMISSIONS_REQUEST_CODE);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        Log.i(TAG, "onRequestPermissionResult");
        if (requestCode == REQUEST_PERMISSIONS_REQUEST_CODE) {
            if (grantResults.length <= 0) {
                Log.i(TAG, "User interaction was cancelled.");
            } else if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                mService.starTracking();
            } else {
                // Permission denied.
                Snackbar.make(
                        findViewById(R.id.activity_main),
                        R.string.permission_denied_explanation,
                        Snackbar.LENGTH_INDEFINITE)
                        .setAction(R.string.action_settings, new View.OnClickListener() {
                            @Override
                            public void onClick(View view) {
                                // Build intent that displays the App settings screen.
                                Intent intent = new Intent();
                                intent.setAction(
                                        Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
                                Uri uri = Uri.fromParts("package",
                                        BuildConfig.APPLICATION_ID, null);
                                intent.setData(uri);
                                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                                startActivity(intent);
                            }
                        })
                        .show();
            }
        }
    }
}
