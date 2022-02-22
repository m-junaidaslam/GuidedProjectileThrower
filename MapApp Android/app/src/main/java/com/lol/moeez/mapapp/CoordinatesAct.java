package com.lol.moeez.mapapp;

public class CoordinatesAct {
    private double lat,lng;

    public CoordinatesAct(){}

    public CoordinatesAct(double lt, double ln){
       lat = lt; lng = ln;
    }

    public double get_lat(){
       return lat;
    }

    public double get_lng(){
        return lng;
    }
}


