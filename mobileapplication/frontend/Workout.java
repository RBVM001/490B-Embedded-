package com.example.testappver1;

public class Workout {
    String Date;
    String Time;
    String Distance;
    String Steps;
    String Calories;

    public Workout(String date, String time, String distance, String steps, String calories) {
        this.Date = date;
        this.Time = time;
        this.Distance = distance;
        this.Steps = steps;
        this.Calories = calories;
    }

    public String getDate() {
        return Date;
    }

    public String getTime() {
        return Time;
    }

    public String getDistance() {
        return Distance;
    }

    public String getSteps() {
        return Steps;
    }

    public String getCalories() {
        return Calories;
    }
}
