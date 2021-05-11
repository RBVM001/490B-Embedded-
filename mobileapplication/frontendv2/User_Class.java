package com.example.finaldemo;


public class User_Class {
    private int id;
    private String name, email, userName, password;
    private int weight, height, age;

    public User_Class(String name, String email, String userName, String password, int weight, int height, int age) {
        this.name = name;
        this.email = email;
        this.userName = userName;
        this.password = password;
        this.weight = weight;
        this.height = height;
        this.age = age;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getEmail() {
        return email;
    }

    public String getUserName() {
        return userName;
    }

    public String getPassword() {
        return password;
    }

    public int getWeight() {
        return weight;
    }

    public int getHeight() {
        return height;
    }

    public int getAge() {
        return age;
    }
}
