package com.tomicooler.bmw.e46track;

import android.content.Context;
import androidx.preference.PreferenceManager;


class Utils {

    private static final String KEY_REQUESTING_LOCATION_UPDATES = "requesting_locaction_updates";

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

}