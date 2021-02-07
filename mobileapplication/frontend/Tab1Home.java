package com.example.testappver1;

import android.content.Intent;
import android.os.Bundle;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.Chronometer;

public class Tab1Home  extends Fragment{

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        Button button;

        View rootView = inflater.inflate(R.layout.tab1home, container, false);
        button = (Button) rootView.findViewById(R.id.startBtn);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openCurrentWorkout();
            }
        });
        return rootView;
    }
    public void openCurrentWorkout(){
        Intent intent = new Intent(getActivity(), CurrentWorkout.class);
        startActivity(intent);
    }




}
