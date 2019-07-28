package com.androsov.ledcontrolapp;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements CompoundButton.OnCheckedChangeListener {

    private static final String TAG = "LedControlApp";

    private static int LED1 = 0;
    private static int LED2 = 1;
    private static int LED3 = 2;
    private static int LED4 = 3;
    private static int LED_OFF = 0;
    private static int LED_ON = 1;

    com.androsov.ledcontrolservice.ILedControlService LedControlService;
    private boolean isBound = false;

    Switch swch_led1;
    Switch swch_led2;
    Switch swch_led3;
    Switch swch_led4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        swch_led1 = findViewById(R.id.swch1);
        swch_led1.setOnCheckedChangeListener(this);

        swch_led2 = findViewById(R.id.swch2);
        swch_led2.setOnCheckedChangeListener(this);

        swch_led3 = findViewById(R.id.swch3);
        swch_led3.setOnCheckedChangeListener(this);

        swch_led4 = findViewById(R.id.swch4);
        swch_led4.setOnCheckedChangeListener(this);

        lockSwitches();

        try {
            BindToService();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public void checkInitState() {
        int state;

        try {
            state = LedControlService.getLedStatus(LED1);
            if (state == LED_ON) {
                swch_led1.setText(R.string.switch_on_caption);
                swch_led1.setChecked(true);
            }

            state = LedControlService.getLedStatus(LED2);
            if (state == LED_ON) {
                swch_led2.setText(R.string.switch_on_caption);
                swch_led2.setChecked(true);
            }

            state = LedControlService.getLedStatus(LED3);
            if (state == LED_ON) {
                swch_led3.setText(R.string.switch_on_caption);
                swch_led3.setChecked(true);
            }

            state = LedControlService.getLedStatus(LED4);
            if (state == LED_ON) {
                swch_led4.setText(R.string.switch_on_caption);
                swch_led4.setChecked(true);
            }

        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton compoundButton, boolean b) {

        if (!isBound) return;

        try {
            if (b) {
                LedControlService.setLedStatus(getLedNumBySwitch(compoundButton), LED_ON);
                compoundButton.setText(R.string.switch_on_caption);
            } else {
                LedControlService.setLedStatus(getLedNumBySwitch(compoundButton), LED_OFF);
                compoundButton.setText(R.string.switch_off_caption);
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private int getLedNumBySwitch(View swch) {
        switch (swch.getId()) {
            case R.id.swch1:
                return LED1;
            case R.id.swch2:
                return LED2;
            case R.id.swch3:
                return LED3;
            case R.id.swch4:
                return LED4;
        }
        return 0;
    }

    private void lockSwitches() {
        swch_led1.setEnabled(false);
        swch_led2.setEnabled(false);
        swch_led3.setEnabled(false);
        swch_led4.setEnabled(false);
    }

    private void unlockSwitches() {
        swch_led1.setEnabled(true);
        swch_led2.setEnabled(true);
        swch_led3.setEnabled(true);
        swch_led4.setEnabled(true);
    }

    private ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Log.d(TAG, "-> onServiceConnected()");
            LedControlService = com.androsov.ledcontrolservice.ILedControlService.Stub.asInterface(service);
            unlockSwitches();
            checkInitState();
            isBound = true;
        }

        public void onServiceDisconnected(ComponentName className) {
            Log.d(TAG, "-> onServiceDisconnected()");
            LedControlService = null;
            isBound = false;
            lockSwitches();

            Toast.makeText(getApplicationContext(), "Reconnect to service in 5 sec", Toast.LENGTH_LONG).show();
            final Handler handler = new Handler();
            handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    try {
                        BindToService();
                    } catch (ClassNotFoundException e) {
                        e.printStackTrace();
                    }
                }
            }, 5000);
        }
    };

    private void BindToService() throws ClassNotFoundException {
        Intent intent = new Intent("com.androsov.ledcontrolservice.LedControlService");
        intent.setPackage("com.androsov.ledcontrolservice");
        intent.setAction("com.androsov.ledcontrolservice.LedControlService");

        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
    }

}
