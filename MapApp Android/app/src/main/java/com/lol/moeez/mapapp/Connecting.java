package com.lol.moeez.mapapp;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.util.UUID;

/**
 * Created by Moeez on 2/28/2016.
 */
public class Connecting extends Thread {

    private final BluetoothSocket mmSocket;
    private final BluetoothDevice mmDevice;
    //private BluetoothAdapter BA;
    Connected mConnected;
    String string;
    Handler hndlr;
    AllMightyClass myServ;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    public Connecting(BluetoothDevice device, Handler msgHandler) {
        // Use a temporary object that is later assigned to mmSocket,
        // because mmSocket is final
        BluetoothSocket tmp = null;
        mmDevice = device;
        //BA=ba1;
        hndlr = msgHandler;
       // string=strr;
       // myServ = myService;

        // Get a BluetoothSocket to connect with the given BluetoothDevice
        try {
            // MY_UUID is the app's UUID string, also used by the server code
            tmp =mmDevice.createRfcommSocketToServiceRecord(MY_UUID);
            Log.d("Connecting", "start");
        } catch (IOException e) { }
        mmSocket = tmp;
    }

    public void run() {
        // Cancel discovery because it will slow down the connection



        try {
            // Connect the device through the socket. This will block
            // until it succeeds or throws an exception
            mmSocket.connect();
        } catch (IOException connectException) {
       /*     // Unable to connect; close the socket and get out
            try {
                mmSocket.close();
                Log.d("Connecting", "mid1");
            } catch (IOException closeException) { }
            return;
       */ }

        // Do work to manage the connection (in a separate thread)
        Log.d("Connecting", "mid");
        manageConnectedSocket(mmSocket);
    }

    public  void Write(){
        mConnected.write();
    }

    private void manageConnectedSocket(BluetoothSocket mmSocket) {
        mConnected = new Connected(mmSocket,hndlr,string,myServ);
        Log.d("Connecting", "end");
        mConnected.start();
    }

    public int CheckSocket() {
        if(mmSocket.isConnected())
            return 1;
        else
            return 0;
    }

    public void stopit() {
        try {
            mmSocket.close();
        } catch (IOException e) {

        }
    }

}
