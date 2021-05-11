package com.example.finaldemo;

import com.google.gson.annotations.SerializedName;

public class Workout {
    private int id;
    private String elapsed_time;
    private int stepCount;
    private String createdAt;
    private String updatedAt;

    public Workout(int id, String elapsed_time, int stepCount) {
        this.id = id;
        this.elapsed_time = elapsed_time;
        this.stepCount = stepCount;
    }

    public Workout() {

    }
    public int getStepCount(){
        return stepCount;
    }
    public int getId() { return id; }
    public String getElapsed_time() {
        return elapsed_time;
    }
    public String getCreatedAt() {
        return createdAt;
    }
    public String getUpdatedAt() { return updatedAt; }
}

/*package com.example.testappver1;

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

}*/
