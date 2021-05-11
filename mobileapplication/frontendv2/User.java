package com.example.finaldemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class User extends AppCompatActivity {
    private String BASE_URL = "http://192.168.254.17:3000";
    //private String BASE_URL = "http://10.0.2.2:3000";
    Retrofit retroFit = new Retrofit.Builder()
            .baseUrl(BASE_URL) // local host
            .addConverterFactory(GsonConverterFactory.create())
            .build();
    RetrofitInterface retrofitInterface = retroFit.create(RetrofitInterface.class);
    int user_id, user_height;
    String user_name;
    EditText name, email, password, weight, height, age;
    Button regButton;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);
        name = findViewById(R.id.nameEdit);
        email = findViewById(R.id.emailEdit);
        password = findViewById(R.id.passwordEdit);
        weight = findViewById(R.id.weightEdit);
        height = findViewById(R.id.heightEdit);
        age = findViewById(R.id.ageEdit);
        regButton = findViewById(R.id.buttonReg);

        regButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                User_Class user = new User_Class(name.getText().toString(), email.getText().toString(),"user", password.getText().toString(), Integer.parseInt(weight.getText().toString()), Integer.parseInt(height.getText().toString()), Integer.parseInt(age.getText().toString()));
                Call<User_Class> call = retrofitInterface.executeSignup(user);
                call.enqueue(new Callback<User_Class>() {
                    @Override
                    public void onResponse(Call<User_Class> call, Response<User_Class> response) {
                        if (!response.isSuccessful()) {
                            Toast.makeText(getApplicationContext(), "User registration is not successful" + response, Toast.LENGTH_SHORT).show();
                        }
                        if (response.code()==200){
                            Toast.makeText(getApplicationContext(), "Signed up successfully", Toast.LENGTH_LONG).show();
                            User_Class user = response.body();
                            user_id = user.getId();
                            user_name = user.getName();
                            user_height = user.getHeight();
                            Intent intent = new Intent(getApplicationContext(), MainActivity.class);
                            intent.putExtra("USER_ID", user_id);
                            intent.putExtra("USER_NAME", user_name);
                            intent.putExtra("USER_HEIGHT", user_height);
                            //Toast.makeText(getApplicationContext(), "THIS IS USER ID: " + user_id, Toast.LENGTH_SHORT).show();
                            //Toast.makeText(getApplicationContext(), "HEIGHT: " + user_height, Toast.LENGTH_SHORT).show();
                            startActivity(intent);
                        }
                        else if (response.code() == 400){
                            Toast.makeText(getApplicationContext(), "Already Registered", Toast.LENGTH_LONG).show();
                        }
                    }

                    @Override
                    public void onFailure(Call<User_Class> call, Throwable t) {

                    }
                });

            }
        });
    }

}