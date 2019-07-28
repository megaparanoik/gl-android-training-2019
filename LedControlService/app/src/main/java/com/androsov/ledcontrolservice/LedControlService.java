package com.androsov.ledcontrolservice;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class LedControlService extends Service {

    private static final String TAG = "LedControlSrvc:Srvc";
    private LedControlServiceImpl mBinder = null;

    public LedControlService() {
    }


    @Override
    public void onCreate() {
        super.onCreate();
        Log.d(TAG, "-> onCreate() ");

        mBinder = new LedControlServiceImpl();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "-> onStartCmd()");
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.d(TAG, "-> onBind()");
        return mBinder;
    }

}
