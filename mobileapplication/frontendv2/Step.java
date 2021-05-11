package com.example.finaldemo;

public class Step {
    private int id;
    private Character side;
    private String val;
    private String time;
    private int workoutId;
    private String createdAt;
    private int currentStep;
    private int position;

    public Step(Character side, String val, String time, int currentStep, int position) {
        this.side = side;
        this.val = val;
        this.currentStep = currentStep;
        this.time = time;
        this.position = position;
    }

    public Step(){

    }

    public int getId() {
        return id;
    }
    public String getTime(){
        return time;
    }
    public Character getSide() {
        return side;
    }
    public int getCurrentStep() {return  currentStep;}
    public int getPosition() {return position;}

    public String getVal() {
        return val;
    }

    public int getWorkoutId() {
        return workoutId;
    }

    public String getCreatedAt() {
        return createdAt;
    }
}
