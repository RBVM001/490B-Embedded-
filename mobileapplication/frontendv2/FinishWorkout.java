package com.example.finaldemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;
import android.util.Log;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class FinishWorkout extends AppCompatActivity {
    private String BASE_URL = "http://192.168.254.17:3000";
    //private String BASE_URL = "http://10.0.2.2:3000";
    Retrofit retroFit = new Retrofit.Builder()
            .baseUrl(BASE_URL) // local host
            .addConverterFactory(GsonConverterFactory.create())
            .build();
    RetrofitInterface retrofitInterface = retroFit.create(RetrofitInterface.class);
    private Button leftButton, homeButton, rightButton;
    private int workoutId, stepCount = 0, StepCount;
    //private ArrayList<Integer> integerArrayList = new ArrayList<>();
    private TextView tTime, tDistance, tCalories, tSteps;
    private ArrayList<Integer> integerArrayListLeft = new ArrayList<>();
    private ArrayList<Integer> integerArrayListRight = new ArrayList<>();
    private String inString, time;
    char side;
    private int firstStepId, currentStep, lastStepId, totalSteps, location;
    private String elaspedTime;
    String distanceTraveled;
    private Step steps;
    private TextView stepIt;
    private TextView stepIdText, stepSideText, stepTimeText, stepWorkoutId;
    // instantiate the matrix
    ArrayList<View> SquareListR = new ArrayList<>();
    ArrayList<View> SquareListL = new ArrayList<>();
    private int RightCounter = 0;
    private int LeftCounter = 0;
    private int currentCurrentStep = 0;
    private String currentCurrentTime, userName;
    private int userId, userHeight;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_finish_workout);
/*        userId = getIntent().getIntExtra("USERS", 0);
        Intent intent = new Intent(this, MainActivity.class);
        intent.putExtra("USER_ID", userId);*/

        tSteps = findViewById(R.id.textSteps);
        tTime = findViewById(R.id.textTime);
        tDistance = findViewById(R.id.textDistance);
        Intent mIntent = this.getIntent();
        Bundle extras = mIntent.getExtras();
        workoutId = extras.getInt("WORKOUT_ID");
        StepCount = extras.getInt("STEP_COUNT");
        totalSteps = extras.getInt("TOTAL_STEPS");
        elaspedTime = extras.getString("ELAPSED_TIME");
        distanceTraveled = extras.getString("DISTANCE");
        userId = extras.getInt("USER_ID");
        userHeight = extras.getInt("USER_HEIGHT");
        userName = extras.getString("USER_NAME");

        //Toast.makeText(getApplicationContext(), "TRAVELED: " + distanceTraveled, Toast.LENGTH_SHORT).show();
        //Toast.makeText(this, "USER HEIGHT: " + userHeight, Toast.LENGTH_SHORT).show();
        tDistance.setText(distanceTraveled);
        //Log.e("Total Steps", String.valueOf(totalSteps));
        //Log.e("elapsed_time", elaspedTime);
        tSteps.setText(String.valueOf(totalSteps));
        tTime.setText(elaspedTime);
        // right sole
        for (int i = -1; i < 255; i++) {
            String squareID = "t" + (i + 1);
            int resID = getResources().getIdentifier(squareID, "id", getPackageName());
            SquareListR.add(((View) findViewById(resID)));   // add all View type to ArrayList
        }
        // left sole
        for (int i = -1; i < 255; i++) {
            String squareID = "s" + (i + 1);
            int resID = getResources().getIdentifier(squareID, "id", getPackageName());
            SquareListL.add(((View) findViewById(resID)));   // add all View type to ArrayList
        }
        stepIt = findViewById(R.id.stepIteration);
        stepIt.setText("Step: " + currentCurrentStep);
        Call<Step> call = retrofitInterface.getFirstStep(workoutId);
        call.enqueue(new Callback<Step>() {
            @Override
            public void onResponse(Call<Step> call, Response<Step> response) {
                if (!response.isSuccessful()) {
                    Toast.makeText(getApplicationContext(), "No Success", Toast.LENGTH_SHORT).show();
                }
                steps = response.body();
                firstStepId = 0;

                getLastStep();
                Toast.makeText(getApplicationContext(), "LAST STEP ID: " + lastStepId, Toast.LENGTH_SHORT).show();
                //Toast.makeText(getApplicationContext(), "First step: " + String.valueOf(firstStepId), Toast.LENGTH_SHORT).show();
                currentStep = firstStepId;
                inString = steps.getVal();
                stepIt.setText("Step: " + steps.getCurrentStep());
                stepIdText.setText("Step ID: " + steps.getId());
                stepSideText.setText("Side: " + steps.getSide());
                stepTimeText.setText("Time: " + steps.getTime());
                stepWorkoutId.setText("Workout ID: " + steps.getWorkoutId());
                side = steps.getSide();
                if (side == 'L')
                    colorMatrixLeft(inString);
                else if (side == 'R')
                    colorMatrixRight(inString);
            }
            @Override
            public void onFailure(Call<Step> call, Throwable t) {
                Toast.makeText(getApplicationContext(), "Failed. " + t.getCause(),Toast.LENGTH_SHORT).show();
            }
        });

        // use last step id to fetch the current step count to update steps in Workout object (new code, dunno work)
/*        Call<List<Step>> call1 = retrofitInterface.getAStep(workoutId, lastStepId -1 );
        call1.enqueue(new Callback<List<Step>>() {
            @Override
            public void onResponse(Call<List<Step>> call, Response<List<Step>> response) {
                if (!response.isSuccessful())
                    Toast.makeText(getApplicationContext(), "Getting Last Step is not successful", Toast.LENGTH_SHORT).show();
                List<Step> stepList = response.body();
                for (Step step : stepList){
                    currentCurrentStep = step.getCurrentStep();
                    currentCurrentTime = step.getTime();
                    Toast.makeText(getApplicationContext(), "CURRENT CURRENT TIME: " + currentCurrentTime, Toast.LENGTH_SHORT).show();
                }
                Toast.makeText(getApplicationContext(), "TTIME: " + tTime, Toast.LENGTH_SHORT).show();
                tTime.setText("" + currentCurrentTime);
                tSteps.setText("" + currentCurrentStep + " steps");
            }

            @Override
            public void onFailure(Call<List<Step>> call, Throwable t) {

            }
        });*/

        // calculate distance
        // need to do. i need to grab height from user in order to calculate stride length and find distance traveled.

        // viewById
        // TEXT FIELDS
        stepIdText = findViewById(R.id.stepIdText);
        stepSideText = findViewById(R.id.stepSideText);
        stepTimeText = findViewById(R.id.stepTimeText);
        stepWorkoutId = findViewById(R.id.stepWorkoutId);

        homeButton = findViewById(R.id.homebtn);
        homeButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                openMainActivity();
            }
        });
        rightButton = findViewById(R.id.rightTriangle);
        rightButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!(location == lastStepId)) {
                    //stepCount += 1; // for stepIt TextView
                    //currentStep = currentStep + 1;
                    location++;
                    getNextStep();
                } else {
                    Toast.makeText(getApplicationContext(), "LAST step already reached", Toast.LENGTH_SHORT).show();
                }
            }
        });
        leftButton = findViewById(R.id.leftTriangle);
        leftButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!(location == firstStepId)) {
                    //stepCount -= 1;
                   // currentStep = currentStep - 1;
                    location--;
                    getNextStep();
                } else {
                    Toast.makeText(getApplicationContext(), "FIRST step already reached", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
/*    private void colorMatrix(String inString) {
        int start = 1;
        int end = 4;
        integerArrayList.clear();
*//*        for (int i = 0; i < 121; i++) {
            String a = inString.substring(start,end);
            integerArrayList.add(Integer.parseInt(a));
            start += 4;
            end += 4;
        }*//*
        for (int i = 0; i < SquareList.size(); i++) {
            if (integerArrayList.get(i) <= 0)
                SquareList.get(i).setBackgroundColor(0xFFFFFFFF);
            else if (integerArrayList.get(i) < 65)
                SquareList.get(i).setBackgroundColor(0xFFD65F59);
            else if (integerArrayList.get(i) < 130)
                SquareList.get(i).setBackgroundColor(0xFFBD1628);
            else if (integerArrayList.get(i) < 190)
                SquareList.get(i).setBackgroundColor(0xFF8D0101);
            else if (integerArrayList.get(i) < 230)
                SquareList.get(i).setBackgroundColor(0xFF680101);
            else
                SquareList.get(i).setBackgroundColor(0xFF420C09);
        }
    }*/

    private void colorMatrixRight(String inString){
        //editCalories.setText("" + RightCounter);
        integerArrayListRight.clear();
        int start = 0;
        int end = 3;
        for (int i = 0; i < 256; i++) {
            String a = inString.substring(start, end);
            integerArrayListRight.add(Integer.parseInt(a));
            start+=3;
            end+=3;
        }
       // Toast.makeText(this, "Right ArrayList: " + integerArrayListRight, Toast.LENGTH_SHORT).show();
        for (int i = 0; i < SquareListR.size(); i++) {
            if (integerArrayListRight.get(i) <= 0)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFFFFFFF);
            else if (integerArrayListRight.get(i) < 65)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFD65F59);
            else if (integerArrayListRight.get(i) < 130)
                SquareListR.get(RightCounter).setBackgroundColor(0xFFBD1628);
            else if (integerArrayListRight.get(i) < 190)
                SquareListR.get(RightCounter).setBackgroundColor(0xFF8D0101);
            else if (integerArrayListRight.get(i) < 230)
                SquareListR.get(RightCounter).setBackgroundColor(0xFF680101);
            else
                SquareListR.get(RightCounter).setBackgroundColor(0xFF420C09);
            RightCounter++;
        }
        if (integerArrayListRight.get(255) == 255 || RightCounter == 256) {
            RightCounter = 0;
        }
    }
    private void colorMatrixLeft(String inString){
        //editCalories.setText("" + counter);
        integerArrayListLeft.clear();
        int start = 0;
        int end = 3;
        for (int i = 0; i < 256; i++) {
            String a = inString.substring(start, end);
            integerArrayListLeft.add(Integer.parseInt(a));
            start+=3;
            end+=3;
        }
        //Toast.makeText(this, "Left ArrayList: " + integerArrayListLeft, Toast.LENGTH_SHORT).show();
        for (int i = 0; i < SquareListL.size(); i++) {
            if (integerArrayListLeft.get(i) <= 0)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFFFFFFF);
            else if (integerArrayListLeft.get(i) < 65)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFD65F59);
            else if (integerArrayListLeft.get(i) < 130)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFFBD1628);
            else if (integerArrayListLeft.get(i) < 190)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF8D0101);
            else if (integerArrayListLeft.get(i) < 230)
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF680101);
            else
                SquareListL.get(LeftCounter).setBackgroundColor(0xFF420C09);
            LeftCounter++;
        }
        if (integerArrayListLeft.get(255) == 255 || LeftCounter == 256) {
            LeftCounter = 0;
        }
    }
    public void getNextStep() {
        Call<List<Step>> call = retrofitInterface.getAStep(workoutId, location);
        call.enqueue(new Callback<List<Step>>() {
            @Override
            public void onResponse(Call<List<Step>> call, Response<List<Step>> response) {
                if (!response.isSuccessful()) {
                    Toast.makeText(getApplicationContext(), "No Success", Toast.LENGTH_SHORT).show();
                }
                List<Step> listStep = response.body();
                String content = "";
                for (Step list : listStep) {
                    currentStep = list.getId();
                    inString = list.getVal();
                    side = list.getSide();
                    time = list.getTime();
                    workoutId = list.getWorkoutId();
                    currentCurrentStep = list.getCurrentStep();
                    Log.e("currentStep", String.valueOf(currentCurrentStep));
                }
                stepIt.setText("Step: " + currentCurrentStep);
                stepIdText.setText("Step ID: " + currentStep);
                stepSideText.setText("Side: " + side);
                stepTimeText.setText("Time: " + time);
                stepWorkoutId.setText("Workout ID: " + workoutId);
                //colorMatrixRight(inString);
                if (side == 'L')
                    colorMatrixLeft(inString);
                else if (side == 'R')
                    colorMatrixRight(inString);
            }
            @Override
            public void onFailure(Call<List<Step>> call, Throwable t) {
                Toast.makeText(getApplicationContext(), "Failed. " + t.getCause(),Toast.LENGTH_SHORT).show();
            }
        });
    }
    public void getLastStep(){
        lastStepId = StepCount - 1;
    }
    public void openMainActivity(){
        Intent intent = new Intent(this, MainActivity.class);
        intent.putExtra("USER_NAME", userName);
        intent.putExtra("USER_ID", userId);
        intent.putExtra("USER_HEIGHT", userHeight);
        startActivity(intent);
    }
}