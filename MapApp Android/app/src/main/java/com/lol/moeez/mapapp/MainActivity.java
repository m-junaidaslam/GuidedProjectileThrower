package com.lol.moeez.mapapp;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.lol.moeez.mapapp.List.MyCostumRow;
import com.lol.moeez.mapapp.List.MyListAdapter;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class MainActivity extends Activity {

    Button on, off, vis, list;
    List<MyCostumRow> data;
    ListView listView;
    Context context;
    private BluetoothAdapter ba;
    private Set<BluetoothDevice> pd;
    BluetoothDevice device;
    Intent intent;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bt);

        context = this;
        on= (Button) findViewById(R.id.on);
        off= (Button) findViewById(R.id.off);
        vis= (Button) findViewById(R.id.vis);
        list= (Button) findViewById(R.id.listB);
        listView= (ListView) findViewById(R.id.listView);
        data = new ArrayList<MyCostumRow>();

        ba= BluetoothAdapter.getDefaultAdapter();
        ListDevices();

        on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                On();
            }
        });

        off.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Off();
            }
        });

        vis.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Visible();
            }
        });

        list.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                ListDevices();
            }
        });

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void On() {
        if(!ba.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn,0);
            toastIt("Turning On");
        } else
            toastIt("Already On");
    }

    private void Off() {
        ba.disable();
        toastIt("Turning Off");
    }

    private void Visible() {
        Intent getVisible = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        startActivityForResult(getVisible, 0);
    }

    private void ListDevices() {

        ba.startDiscovery();
         final BroadcastReceiver mReceiver = new BroadcastReceiver() {
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                // When discovery finds a device
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    // Get the BluetoothDevice object from the Intent
                    device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    // Add the name and address to an array adapter to show in a ListView
                    data.add(new MyCostumRow(device.getName(),device.getAddress(),device));
                }
            }
        };
// Register the BroadcastReceiver

        pd = ba.getBondedDevices();
        for(BluetoothDevice bt : pd) {
            String name = bt.getName().toString();
            String address = bt.getAddress().toString();
            data.add(new MyCostumRow(name, address, bt));
        }

        toastIt("Showing Available Devices");

        ListAdapter adapter =  new MyListAdapter(context, R.layout.activity_mylistadapter, data);
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                ba.cancelDiscovery();
                intent = new Intent(MainActivity.this,MapsActivity.class);
                intent.putExtra("BTdev",data.get(position).getDevice());
                startActivity(intent);

            }
        });
    }

    public void toastIt(String s) {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }


}


