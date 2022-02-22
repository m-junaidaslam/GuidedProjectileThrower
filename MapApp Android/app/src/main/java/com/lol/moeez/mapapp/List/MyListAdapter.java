package com.lol.moeez.mapapp.List;

import android.app.Activity;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.lol.moeez.mapapp.R;

import java.util.List;

/**
 * Created by Moeez on 2/28/2016.
 */
public class MyListAdapter extends ArrayAdapter<MyCostumRow> {
    Activity context;
    int resource;
    List<MyCostumRow> data;

    public MyListAdapter(Context context, int resource, List<MyCostumRow> data) {
        super(context, resource, data);
        this.context = (Activity) context;
        this.resource = resource;
        this.data = data;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        Holder holder = null;
        View row = convertView;

        if (row == null) {
            LayoutInflater inflater = context.getLayoutInflater();
            row = inflater.inflate(resource, parent, false);

            holder= new Holder();
            holder.txtView1 = (TextView) row.findViewById(R.id.textView1);
            holder.txtView2 = (TextView) row.findViewById(R.id.textView2);

            row.setTag(holder);
        } else {
            holder = (Holder) row.getTag();
        }

        String str1 = data.get(position).getName();
        holder.txtView1.setText(str1.toString());
        String str2= data.get(position).getMAC();
        holder.txtView2.setText(str2.toString());

        return row;
    }


    static class Holder {
        TextView txtView1;
        TextView txtView2;
    }
}
