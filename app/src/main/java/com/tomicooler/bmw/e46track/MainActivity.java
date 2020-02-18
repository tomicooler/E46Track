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
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

import android.Manifest;

import android.content.pm.PackageManager;

import android.net.Uri;

import android.provider.Settings;

import android.widget.TextView;

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
            mService.getModel().getCurrentLocation().observeForever(new Observer<Location>() {
                @Override
                public void onChanged(Location location) {
                    latitude.setText(String.format(Locale.getDefault(), "%f", location.getLatitude()));
                    longitude.setText(String.format(Locale.getDefault(), "%f", location.getLongitude()));
                    altitude.setText(String.format(Locale.getDefault(), "%f", location.getAltitude()));
                    speed.setText(String.format(Locale.getDefault(), "%f", location.getSpeed() * 3.6));
                    bearing.setText(String.format(Locale.getDefault(), "%f", location.getBearing()));
                }
            });

            final TextView oilTemp = findViewById(R.id.oilTemp);
            mService.getModel().getCurrentOilTemp().observeForever(new Observer<Double>() {
                @Override
                public void onChanged(Double oil) {
                    oilTemp.setText(String.format(Locale.getDefault(), "%f", oil));
                }
            });

            final TextView clutch = findViewById(R.id.clutch);
            mService.getModel().getCurrentClutch().observeForever(new Observer<Boolean>() {
                @Override
                public void onChanged(Boolean on) {
                    clutch.setText(on ? "1" : "0");
                }
            });

            final TextView brake = findViewById(R.id.brake);
            mService.getModel().getCurrentBrake().observeForever(new Observer<Boolean>() {
                @Override
                public void onChanged(Boolean on) {
                    brake.setText(on ? "1" : "0");
                }
            });

            final TextView throttle = findViewById(R.id.throttle);
            mService.getModel().getCurrentThrottle().observeForever(new Observer<Double>() {
                @Override
                public void onChanged(Double value) {
                    throttle.setText(String.format(Locale.getDefault(), "%f", value));
                }
            });

            final TextView steeringAgle = findViewById(R.id.steeringAngle);
            mService.getModel().getCurrentSteeringAngle().observeForever(new Observer<Double>() {
                @Override
                public void onChanged(Double value) {
                    steeringAgle.setText(String.format(Locale.getDefault(), "%f", value));
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
