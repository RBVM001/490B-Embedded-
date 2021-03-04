package com.example.testappver1;
import android.content.Intent;
import android.os.Bundle;
import androidx.fragment.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import com.example.testappver1.ui.main.WorkoutAdapter;

import java.util.ArrayList;

public class Tab2SavedWorkouts extends Fragment{

    ListView listView;      // create a listview object

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState){
        View rootView = inflater.inflate(R.layout.tab2savedworkouts, container, false);

        listView = rootView.findViewById(R.id.listView);
        final ArrayList<Workout> arrayList = new ArrayList<>();
        WorkoutAdapter workoutAdapter;
        arrayList.add(new Workout("January 1, 2021", "1h 40 min", "5.05 mi", "10302", "532"));
        arrayList.add(new Workout("February 14, 2021", "50 min", "2.55 mi", "536", "442"));
        arrayList.add(new Workout("March 17, 2021", "2h 11 min", "7.05 mi", "15302", "838"));
        arrayList.add(new Workout("April 20, 2021", "46 min", "3.11 mi", "6062", "461"));
        arrayList.add(new Workout("May 4, 2021", "1h 40 min", "1.04 mi", "2056", "157"));
        arrayList.add(new Workout("January 1, 2021", "1h 40 min", "5.05 mi", "10302", "532"));
        arrayList.add(new Workout("February 14, 2021", "50 min", "2.55 mi", "536", "442"));
        arrayList.add(new Workout("March 17, 2021", "2h 11 min", "7.05 mi", "15302", "838"));
        arrayList.add(new Workout("April 20, 2021", "46 min", "3.11 mi", "6062", "461"));
        arrayList.add(new Workout("May 4, 2021", "1h 40 min", "1.04 mi", "2056", "157"));

        workoutAdapter = new WorkoutAdapter(this.getContext(), R.layout.list_row, arrayList);
        listView.setAdapter(workoutAdapter);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener(){

            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                if (position == 0) {
                    Intent intent = new Intent(view.getContext(), FinishWorkout.class);
                    startActivity(intent);
                }
            }
        });

        return rootView;
    }
}
