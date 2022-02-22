package com.lol.moeez.mapapp;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.Circle;
import com.google.android.gms.maps.model.CircleOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {

    LatLng point;
    TextView TV;
    TextView TV1;
    Button start, stop;
    CoordinatesAct CA,CA1 = new CoordinatesAct(0, 0);
    CoordinatesAct locCoords = new CoordinatesAct(0, 0),locCoords1 = new CoordinatesAct(0, 0);
    String Str;
    String theString = "";
    BluetoothDevice BTDev;
    Context context;
    float lati,longi;
    Connecting conn;
    GoogleMap mMap;
    int i = 0;
    String check = "x";
    private StringBuilder sb = new StringBuilder();//new


    Handler msgHandler = new Handler() {

       public void handleMessage(Message msg) {
           switch (msg.what) {
               case 1:                                                    // if receive massage
                   byte[] readBuf = (byte[]) msg.obj;
                   String strIncom = new String(readBuf, 0, msg.arg1);                    // create string from bytes array
                   sb.append(strIncom);                                                // append string
                   int endOfLineIndex = sb.indexOf("\r\n");                            // determine the end-of-line
                   if (endOfLineIndex > 0) {                                            // if end-of-line,
                       String sbprint = sb.substring(0, endOfLineIndex);                // extract string
                       sb.delete(0, sb.length());                                        // and clear
                       switch (check){
                           case "a":
                               if(!sbprint.isEmpty() && (sbprint.contains(".")) && sbprint.contains(",")) {
                                   dataProcess(sbprint);
                                   RefreshMap();
                               }
                               else
                                ToastIt("empty");
                               break;

                           // Here we should add the cases depending on which data type to recieve

                       }
                   }

                   break;
           }
       }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        context = this;
        Intent intent = getIntent();
        BTDev = intent.getParcelableExtra("BTdev");
        TV = (TextView) findViewById(R.id.textView);
        TV1 = (TextView) findViewById(R.id.textView2);
        start = (Button) findViewById(R.id.start);
        stop = (Button) findViewById(R.id.stop);
        stop.setEnabled(false);
        conn = new Connecting(BTDev,msgHandler);
        conn.start();

        start.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AllMightyClass.str = "a";
                check = "a";
                if(conn.CheckSocket() == 0)
                    conn.run();
                conn.Write();
                start.setEnabled(false);
                stop.setEnabled(true);
            }
        });

        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AllMightyClass.str = "b";
                check = "b";
                conn.Write();
                stop.setEnabled(false);
                start.setEnabled(true);
            }
        });

        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }

    /*@Override
    public void onMapReady(final GoogleMap map) {

        uMap = map;
        map.setMyLocationEnabled(true);
        map.getUiSettings().setMyLocationButtonEnabled(true);
        map.getUiSettings().setCompassEnabled(true);
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);


        //Create a criteria object to retrieve provider
        Criteria criteria = new Criteria();

        //Get the name of best provider
        String provider = locationManager.getBestProvider(criteria, true);

        //Get current location

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }

        Location myLocation = locationManager.getLastKnownLocation(provider);

        map.setMapType(GoogleMap.MAP_TYPE_HYBRID);

        if (myLocation != null) {
            ToastIt("Location is received!");
            //Get latitude of current location
            final double latitude = myLocation.getLatitude();

            //Get longitude of current location
            final double longitude = myLocation.getLongitude();

            locCoords1 = new CoordinatesAct(latitude,longitude);
            //locCoords = new CoordinatesAct(lati, longi);

            //Create a LatLng object for current location
            //LatLng latLng = new LatLng(lati, longi);

            //Show current location on google map
            map.moveCamera(CameraUpdateFactory.newLatLng(new LatLng(latitude,longitude)));

            //Zoom in Google Map
            map.moveCamera(CameraUpdateFactory.zoomTo(17));
            map.addMarker(new MarkerOptions().position(new LatLng(latitude, longitude))
                    .title("You are here!").snippet("Consider yourself located"));



            map.setOnMapClickListener(new GoogleMap.OnMapClickListener() {
                @Override
                public void onMapClick(LatLng latLng) {
                    point = latLng;
                    double lat = point.latitude;
                    double lng = point.longitude;

                    Str = "(" + lat + "," + lng + ")";


                    if (i == 0) {
                        TV.setText(Str);
                        CA = new CoordinatesAct(lat, lng);
                        TV1.setText(String.valueOf(lati) + "," + String.valueOf(longi));
                    }
                    if (i == 1) {
                        TV1.setText(Str);
                        CA1 = new CoordinatesAct(lat, lng);
                        double dist = calcDist();
                        String strng = String.valueOf(dist) + " meters";
                        String strng1 = String.valueOf(calcAngle()) + " degrees";

                    }
                    i++;
                    Log.d("Map LATLNG", Str);
                    if (i > 1) i = 0;
                }
            });

        }
    }
*/

    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;

        // Add a marker in Sydney and move the camera
        mMap.addMarker(new MarkerOptions().position(new LatLng(0, 0)).title("Marker").snippet("Snippet"));

        //Enable my location layer of google map
        //mMap.setMyLocationEnabled(true);
        //mMap.getUiSettings().setMyLocationButtonEnabled(true);
        mMap.getUiSettings().setCompassEnabled(true);

        //Get LocationManager object from System Service LOCATION_SERVICE
        //LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

        //Create a criteria object to retrieve provider
        //Criteria criteria = new Criteria();

        //Get the name of best provider
        //String provider = locationManager.getBestProvider(criteria, true);

        //Get current location

        //if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the permission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
        //    return;
        //}
        //Location myLocation = locationManager.getLastKnownLocation(provider);

        //Set map type
        //mMap.setMapType(GoogleMap.MAP_TYPE_SATELLITE);

        //if (myLocation != null) {
            //ToastIt("Location is received!");
            //Get latitude of current location
            //final double latitude = myLocation.getLatitude();

            //Get longitude of current location
            //final double longitude = myLocation.getLongitude();

            //locCoords = new CoordinatesAct(latitude, longitude);

            //Create a LatLng object for current location
            //LatLng latLng = new LatLng(latitude, longitude);

            //Show current location on google map
            //mMap.moveCamera(CameraUpdateFactory.newLatLng(latLng));

            //Zoom in Google Map
            //mMap.moveCamera(CameraUpdateFactory.zoomTo(17));
            //mMap.addMarker(new MarkerOptions().position(new LatLng(latitude, longitude))
             //       .title("You are here!").snippet("Consider yourself located"));

        //} else
            //ToastIt("Location is not received!");

        mMap.setOnMapLongClickListener(new GoogleMap.OnMapLongClickListener() {
            @Override
            public void onMapLongClick(LatLng latLng) {
                final double lat = latLng.latitude;
                final double lng = latLng.longitude;

                // Dialog to show location
                // Replaced by Marker
                AlertDialog.Builder myAlert = new AlertDialog.Builder(context);
                CA1 = new CoordinatesAct(lat, lng);
                double dist = calcDist();
                double angle = calcAngle();
                myAlert.setTitle("Location");
                myAlert.setMessage("Longitude and Latitude = " + lat + ", " + lng + "\nDistance = " + dist + "\nAngle = " + angle);
                myAlert.setNeutralButton("Open in Maps", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Uri uri = Uri.parse("geo:" + lat + "," + lng);
                        Intent it = new Intent(Intent.ACTION_VIEW, uri);
                        startActivity(it);
                        //dialog.dismiss();
                    }
                });

                myAlert.create().show();
                mMap.clear();
                mMap.addMarker(new MarkerOptions().position(new LatLng(lat, lng))
                        .title(lat + "," + lng).snippet("New Point"));
                    /*CoordinatesAct CA = new CoordinatesAct(lat, lng);
                    double dist = calcDist(CA);
                    ToastIt("Distance: " + dist);*/
                if(mMap == null)
                    ToastIt("null map hai");
                else
                    ToastIt("map to theek ha");
            }
        });

    }

    public void RefreshMap(){
        if(lati!=0 && longi!=0) {
            mMap.clear();
            mMap.moveCamera(CameraUpdateFactory.newLatLng(new LatLng(lati, longi)));
            mMap.moveCamera(CameraUpdateFactory.zoomTo(17));
            mMap.addMarker(new MarkerOptions().position(new LatLng(lati, longi))
                    .title("You are here!").snippet("Consider yourself located"));
            Circle cc = mMap.addCircle(new CircleOptions()
                    .center(new LatLng(lati, longi))
                    .radius(50).strokeColor(Color.RED));
        }
    }

    public double calcDist() {
        double lt2 = CA1.get_lat()*(3.1415926535/180);
        double ln2 = CA1.get_lng()*(3.1415926535/180);
        //double lt1 = locCoords.get_lat()*(3.1415926535/180);
        //double ln1 = locCoords.get_lng()*(3.1415926535/180);
        double lt1 = lati*(3.1415926535/180);
        double ln1 = longi*(3.1415926535/180);
        double lat2 = (lt2 - lt1);
        double lng2 = (ln2 - ln1);

        double a = (Math.sin(lat2 / 2) * (Math.sin(lat2 / 2))) + (Math.cos(lt1)) * (Math.cos(lt2)) * (Math.sin(lng2 / 2) * (Math.sin(lng2 / 2)));
        double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        double d = 6371000 * c;
        return d;

    }

    public double calcAngle() {
        double lt2 = CA1.get_lat();
        double ln2 = CA1.get_lng();
        double lt1 = locCoords.get_lat();
        double ln1 = locCoords.get_lng();
        double dLon = (ln2-ln1);
        double y = Math.sin(dLon) * Math.cos(lt2);
        double x = Math.cos(lt1)*Math.sin(lt2) - Math.sin(lt1)*Math.cos(lt2)*Math.cos(dLon);
        double brng = Math.toDegrees((Math.atan2(y, x)));
        brng = (360 - ((brng + 360) % 360));

        return brng;
    }

    private void ToastIt(String s) {
        Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
    }

    public void  dataProcess(String str){
        Log.d("LatiLongi before rape", str);
        if(str.length() == 26 && (str.endsWith("E") || str.endsWith("W"))) {
            lati = Latitude2Decimal(str.substring(0, 9), str.charAt(11));
            longi = Longitude2Decimal(str.substring(13, 23), str.charAt(25));
            ToastIt(lati+" , "+longi);
        }
    }

    static float Latitude2Decimal(String lat, char NS) {
        float med = new Float(0);
        if(lat.contains(","))
            return 0;
        med = Float.parseFloat(lat.substring(2)) / 60.0f;
        med += Float.parseFloat(lat.substring(0, 2));
        if(NS == 'S') {
            med = -med;
        }
        return med;
    }

    static float Longitude2Decimal(String lon, char WE) {
        float med = new Float(0);
        if(lon.contains(","))
            return 0;
        med = Float.parseFloat(lon.substring(3)) / 60.0f;
        med += Float.parseFloat(lon.substring(0, 3));
        if(WE == 'W') {
            med = -med;
        }
        return med;
    }

    @Override
    protected void onDestroy() {
        conn.stopit();
        ToastIt("Socket Closed");
        super.onDestroy();
    }
}
