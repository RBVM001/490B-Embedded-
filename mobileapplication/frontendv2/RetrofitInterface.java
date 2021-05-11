package com.example.finaldemo;

import java.util.List;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Path;

    public interface RetrofitInterface {
        // create a workout
/*        @POST("/workouts/create")*/
/*        Call<Workout> createWorkout();*/
        @POST("/users/{userId}/workouts/create")
        Call<Workout> createWorkout(@Path("userId") int userId);

        // update elapsed time after a workout is finished
        @PUT("/workouts/{id}")
        Call<Workout> putElapsed(@Path("id") int id, @Body Workout workout);

        @PUT("/workouts/{id}")
        Call<Workout> putSteps(@Path("id") int id, @Body Workout workout);

        @PUT("/workouts/{id}")
        Call<Workout> putDistance(@Path("id") int id, @Body Workout workout);

        // get first step of workout
        @GET("/workouts/{workoutId}/firstStep")
        Call<Step> getFirstStep(@Path("workoutId") int workoutId);

        // get all steps from a workout
        @GET("/workouts/{workoutId}/steps")
        Call<List<Step>> getAllSteps(@Path("workoutId") int workoutId);

        // create a step
        @POST("/workouts/{workoutId}/steps/create")
        Call<Step> createStep(@Path("workoutId") int workoutId, @Body Step step);

        // receive an object of Step from a workout
        @GET("/workouts/{workoutId}/steps/{id}/")
        Call<List<Step>> getAStep(@Path("workoutId") int workoutId, @Path("id") int id);

        @GET("/workouts/{workoutId}/steps/{id}")
        Call<Step> setAttributes(@Path("workoutId") int workoutId, @Path("id") int id);

        // sign up
        @POST("/users/signUp") // signup
        //Call<Void> executeSignup(@Body HashMap<String, String> map);
        Call<User_Class> executeSignup(@Body User_Class user);
    }