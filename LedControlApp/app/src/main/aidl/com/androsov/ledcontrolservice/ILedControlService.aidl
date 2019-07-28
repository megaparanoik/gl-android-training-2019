// ILedControlService.aidl
package com.androsov.ledcontrolservice;

// Declare any non-default types here with import statements

interface ILedControlService {
    void setLedStatus(int led, int led_status);
    int getLedStatus(int led);
}
