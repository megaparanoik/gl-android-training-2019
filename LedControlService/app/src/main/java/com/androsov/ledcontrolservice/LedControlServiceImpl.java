package com.androsov.ledcontrolservice;

import android.os.RemoteException;
import android.util.Log;
import vendor.globallogic.ledcontrol.V1_0.ILedControl;

public class LedControlServiceImpl extends com.androsov.ledcontrolservice.ILedControlService.Stub {

    private static final String TAG = "LedControlSrvc:SrvcImpl";
    ILedControl ledHALService = null;

    public LedControlServiceImpl() {
        try {
            ledHALService = ILedControl.getService(true);
            ledHALService.initializeLedControl();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void setLedStatus(int led, int led_status) throws RemoteException {
        Log.d(TAG, "-> setLedStatus()");

        if (ledHALService != null) {
            ledHALService.setLedStatus(led, led_status);
        } else {
            Log.d(TAG, "Can't connect to HAL");
        }
    }


    public int getLedStatus(int led) throws RemoteException {
        Log.d(TAG, "-> getLedStatus()");

        if (ledHALService != null) {
            return ledHALService.getLedStatus(led);
        } else {
            Log.d(TAG, "Can't connect to HAL");
            return -1;
        }
    }

}
