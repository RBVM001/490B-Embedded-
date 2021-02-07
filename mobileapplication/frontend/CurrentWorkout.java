package com.example.testappver1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.View;
import android.widget.Button;
import android.widget.Chronometer;
import android.widget.ToggleButton;

public class CurrentWorkout extends AppCompatActivity {

    private Chronometer chronometer;
    private boolean running;
    private long pauseOffset;       // needed to calculate time diff. chronometer runs in background

    private Button button;          // this is for finish workout button

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_current_workout);

        chronometer = findViewById(R.id.chronometer);
        chronometer.start();        // start time upon opening
        running = true;

        button = findViewById(R.id.finish);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {           // open new activity (finishedworkout) after btn  pressed
                openFinishWorkout();
            }
        });
    }
    public void openFinishWorkout(){
        Intent intent = new Intent(this, FinishWorkout.class);
        chronometer.stop();
        pauseOffset = SystemClock.elapsedRealtime() - chronometer.getBase();
        startActivity(intent);

    }
    public void onToggleClicked(View v){
        boolean on = ((ToggleButton) v).isChecked(); // check toggled button
        if (!on){    // resume chronometer
            chronometer.setBase(SystemClock.elapsedRealtime() - pauseOffset); // when to start counting, when start btn pressed
            chronometer.start();
            running = true;
        }
        else {      // pause chronometer
            chronometer.stop();     // only stops the text, not the chronometer from counting
            pauseOffset = SystemClock.elapsedRealtime() - chronometer.getBase(); // calculate time
            running = false;

        }
    }
}