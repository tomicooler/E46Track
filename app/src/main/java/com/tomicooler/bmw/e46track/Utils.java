package com.tomicooler.bmw.e46track;

import android.content.Context;
import androidx.preference.PreferenceManager;


class Utils {

    private static final String KEY_REQUESTING_LOCATION_UPDATES = "requesting_locaction_updates";
    private static final String KEY_ADDRESS = "address";
    public static final String KEY_PORT = "port";

    static boolean requestingLocationUpdates(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context)
                .getBoolean(KEY_REQUESTING_LOCATION_UPDATES, false);
    }

    static void setRequestingLocationUpdates(Context context, boolean requestingLocationUpdates) {
        PreferenceManager.getDefaultSharedPreferences(context)
                .edit()
                .putBoolean(KEY_REQUESTING_LOCATION_UPDATES, requestingLocationUpdates)
                .apply();
    }

    static String address(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString(KEY_ADDRESS, "192.168.0.10");
    }

    static int port(Context context) {
        return Integer.parseInt(PreferenceManager.getDefaultSharedPreferences(context).getString(KEY_PORT, "35000"));
    }

}