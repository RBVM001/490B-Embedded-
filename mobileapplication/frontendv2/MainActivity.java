package com.example.finaldemo;
// UPDATED 4/30/2021 12:12 pm
// UPDATED 4/30/2021 12:53 pm

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {
    //private String BASE_URL = "http://10.0.2.2:3000";
    private String BASE_URL = "http://192.168.254.17:3000";
    public  static String USER_NAME;
    public  static int USER_ID;
    public  static int USER_HEIGHT;
    Retrofit retroFit = new Retrofit.Builder()
            .baseUrl(BASE_URL) // local host
            .addConverterFactory(GsonConverterFactory.create())
            .build();
    RetrofitInterface retrofitInterface = retroFit.create(RetrofitInterface.class);

    int workoutId, userId, userHeight;
    String userName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button startButton = (Button) findViewById(R.id.startBtn);
        userId = getIntent().getIntExtra("USER_ID", 0);
        userName = getIntent().getStringExtra("USER_NAME");
        userHeight = getIntent().getIntExtra("USER_HEIGHT", 0);
        Toast.makeText(this, "USERHEIGHT: " + userHeight, Toast.LENGTH_SHORT).show();
        Toast.makeText(getApplicationContext(), "ID: "+ userId, Toast.LENGTH_SHORT).show();
        Toast.makeText(getApplicationContext(), "NAME: "+ userName, Toast.LENGTH_SHORT).show();
        //intent2.putExtra("USER_ID", userId);
        //Toast.makeText(getApplicationContext(), "NAME: " + userName, Toast.LENGTH_SHORT).show();
        //Intent intent = new Intent(getApplicationContext(), CurrentWorkout.class);
        //intent.putExtra("USER", user);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openCurrentWorkout();
            }
        });

    }

    private void openCurrentWorkout() {
        final Workout workout = new Workout();
        final Intent intent = new Intent(this, CurrentWorkout.class);
        intent.putExtra("USER_NAME", userName);
        intent.putExtra("USER_ID", userId);
        intent.putExtra("USER_HEIGHT", userHeight);
        Call<Workout> call = retrofitInterface.createWorkout(userId);
        call.enqueue(new Callback<Workout>() {
            @Override
            public void onResponse(Call<Workout> call, Response<Workout> response) {
                if (!response.isSuccessful())
                    Toast.makeText(getApplicationContext(),"Open Current Workout is not successful", Toast.LENGTH_SHORT).show();
                Workout postWorkout = response.body();
                workoutId = postWorkout.getId();
                intent.putExtra("WORKOUT_NUMBER", workoutId);
                startActivity(intent);
            }
            @Override
            public void onFailure(Call<Workout> call, Throwable t) {
                Toast.makeText(getApplicationContext(),"" + t, Toast.LENGTH_SHORT).show();
            }
        });
    }
}