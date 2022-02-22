package com.lol.moeez.mapapp;

import android.bluetooth.BluetoothSocket;
import android.content.IntentFilter;
import android.os.Handler;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Created by Moeez on 2/28/2016.
 */
public class Connected extends Thread{

    private final BluetoothSocket socket;
    private final InputStream mmInStream;
    private final OutputStream mmOutStream;
    private int bytes;
    private byte[] buffer;
    private String theString;
    android.os.Handler handler;
    IntentFilter fltr;
    int MESSAGE_READ = 1;
    String strr;
    AllMightyClass myService;
    public Connected(BluetoothSocket mmSocket, Handler hndlr, String string, AllMightyClass myServ) {
        socket = mmSocket;
        InputStream tmpIn = null;
        OutputStream tmpOut = null;
        handler = hndlr;
        myService = myServ;
        strr=string;
        Log.d("Connected","start");
        // Get the input and output streams, using temp objects because
        // member streams are final
        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) { }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;


    }

    public void run() {
        buffer = new byte[1024];  // buffer store for the stream
         // bytes returned from read()

        // Keep listening to the InputStream until an exception occurs
        while (true) {
            try {
                //if(AllMightyClass.str != "")
                    //write();
                // Read from the InputStream
                bytes = mmInStream.read(buffer);
                byte[] readbf = (byte[]) buffer;
                theString = new String(readbf,0,bytes);
                Log.d("Incoming!!!", theString);
                // Send the obtained bytes to the UI activity
                sendMessage();
                Log.d("Connected","Running");

            } catch (IOException e) {
                break;
            }
        }
        Log.d("Connected","End");
    }


    public void sendMessage(){
        handler.obtainMessage(1, bytes,-1, buffer).sendToTarget();
    }

    public void write() {

        String outString = AllMightyClass.str;
        Log.d("into write", outString);
        outString = outString + "\r\n";
        byte[] outBytes = outString.getBytes();
        try {

            mmOutStream.write(outBytes);
            Log.d("Recieved",outString);


        } catch (IOException e) {
        Log.d("Error Error","Cannot Send");}
    }



    /* Call this from the main activity to shutdown the connection */
    public void cancel() {
        try {
            socket.close();
        } catch (IOException e) { }
    }


    public String getData() {

        return theString;
    }

}



