package com.lol.moeez.mapapp.List;

import android.bluetooth.BluetoothDevice;

/**
 * Created by Moeez on 2/28/2016.
 */
public class MyCostumRow {

    private String name;
    private String MAC;
    private BluetoothDevice device1;

    public MyCostumRow(String name, String MAC,BluetoothDevice device) {
        this.name = name;
        this.MAC = MAC;
        device1 = device;
    }

    public String getName() {
        return name;
    }

    public String getMAC() {
        return MAC;
    }

    public BluetoothDevice getDevice(){ return device1; }
}


