package com.example.testappver1;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.HashMap;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class Log extends AppCompatActivity {
    private Retrofit retrofit;
    private RetrofitInterface retrofitInterface;
    private String BASE_URL = "http://10.0.2.2:3000";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.log);
        //
        Button b;
        b = (Button) findViewById(R.id.b);
        b.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                open();
            }
        });
        retrofit = new Retrofit.Builder()
                .baseUrl(BASE_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        retrofitInterface = retrofit.create(RetrofitInterface.class);

        findViewById(R.id.login).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                handleLoginDialogue();
                
            }
        });

        findViewById(R.id.signup).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                handleSignupDialogue();
            }
        });
    }

    public void open() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
    }

    private void handleLoginDialogue() {
        View view = getLayoutInflater().inflate(R.layout.login_dialogue, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setView(view).show();
        Button loginBtn = view.findViewById(R.id.login);
        final EditText firstEdit = view.findViewById(R.id.firstEdit);
        final EditText username = view.findViewById(R.id.username);
        final EditText emailEdit = view.findViewById(R.id.emailEdit);
        final EditText passwordEdit = view.findViewById(R.id.password);

        loginBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HashMap<String, String> map = new HashMap<>();
/*
                map.put("first_name", firstEdit.getText().toString());
                map.put("last_name", lastEdit.getText().toString());*/
                map.put("username", username.getText().toString());
                map.put("password", passwordEdit.getText().toString());

                Call<LoginResult> call = retrofitInterface.executeLogin(map);
                call.enqueue(new Callback<LoginResult>() {
                    @Override
                    public void onResponse(Call<LoginResult> call, Response<LoginResult> response) {
                        if (response.code()==200){
                            LoginResult result = response.body();
                            AlertDialog.Builder builder1 = new AlertDialog.Builder(Log.this);
                            builder1.setTitle(result.getFirst());
                            builder1.setMessage(result.getEmail());
                            builder1.show();
                            Toast.makeText(Log.this, "Successful Login",
                                    Toast.LENGTH_LONG).show();
                            Intent intent = new Intent(Log.this, MainActivity.class);
                            startActivity(intent);
                        }
                        else if (response.code()==404){
                            Toast.makeText(Log.this, "INCORRECT USERNAME OR PASSWORD",
                                    Toast.LENGTH_LONG).show();
                        }
                    }

                    @Override
                    public void onFailure(Call<LoginResult> call, Throwable t) {
                        Toast.makeText(Log.this, t.getMessage(),
                                Toast.LENGTH_LONG).show();
                    }
                });
            }
        });
    }
    private void handleSignupDialogue(){
        View view = getLayoutInflater().inflate(R.layout.signup_dialouge, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setView(view).show();
        Button signupBtn = view.findViewById(R.id.signup);
        final EditText firstEdit = view.findViewById(R.id.firstEdit);
        final EditText usernameEdit = view.findViewById(R.id.usernameEdit);
        final EditText emailEdit = view.findViewById(R.id.emailEdit);
        final EditText passwordEdit = view.findViewById(R.id.passwordEdit);
        signupBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                HashMap<String, String> map = new HashMap<>();
                map.put("full_name", firstEdit.getText().toString());
                map.put("username", usernameEdit.getText().toString());
                map.put("email", emailEdit.getText().toString());
                map.put("password", passwordEdit.getText().toString());
                Call<Void> call = retrofitInterface.executeSignup(map);
                call.enqueue(new Callback<Void>() {
                    @Override
                    public void onResponse(Call<Void> call, Response<Void> response) {
                        if (response.code()==200){
                            Toast.makeText(Log.this,
                                    "Signed up successfully", Toast.LENGTH_LONG).show();
                        }
                        else if (response.code() == 400){
                            Toast.makeText(Log.this,
                                    "Already Registered", Toast.LENGTH_LONG).show();
                        }
                    }
                    @Override
                    public void onFailure(Call<Void> call, Throwable t) {
                        Toast.makeText(Log.this, t.getMessage(),
                                Toast.LENGTH_LONG).show();
                    }
                });
            }
        });
    }
}