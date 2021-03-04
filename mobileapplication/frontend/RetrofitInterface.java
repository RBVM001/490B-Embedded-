package com.example.testappver1;

import java.util.HashMap;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.POST;

public interface RetrofitInterface {
    @POST("/users/login")
    Call<LoginResult> executeLogin(@Body HashMap<String, String> map);

    @POST("/users/signup") // signup
    Call<Void> executeSignup(@Body HashMap<String, String> map);
}
