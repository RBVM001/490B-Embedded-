module.exports = app => {

// USER - CONTROLLER
    const users = require("../controllers/user.controller.js");
    // Create a new user
    app.post("/users/signUp", users.createUser);

    // Create a new user
    app.post("/users/login", users.loginUser);

    // Retrieve all users
    app.get("/users", users.findAllUsers);

    // Retrieve a single user with id
    app.get("/users/:id", users.findOneUser);

    // Update a user with id
    app.put("/users/:id", users.updateUser);

    // Delete a user with id
    app.delete("/users/:id", users.deleteUser);

    // Delete all users
    app.delete("/users", users.deleteAllUsers);

// WORKOUT - CONTROLLER
    const workouts = require("../controllers/workout.controller.js");
    // Create a new workout
    app.post("/users/:userId/workouts/create", workouts.createWorkout);

    // Retrieve all workouts
    app.get("/users/:userId/workouts", workouts.findAllWorkouts);

    // Retrieve a single workout with id
    app.get("/workouts/:id", workouts.findOneWorkout);

    // Update a workout with id
    app.put("/workouts/:id", workouts.updateWorkout);

    // Delete a workout with id
    app.delete("/workouts/:id", workouts.deleteWorkout);

    // Delete all workouts
    app.delete("/workouts", workouts.deleteAllWorkouts);

// STEPS - CONTROLLER
    const steps = require("../controllers/step.controller.js");
    // Create a new step
    app.post("/workouts/:workoutId/steps/create", steps.createStep);

    // Retrieve all steps
    app.get("/workouts/:workoutId/steps", steps.findAllSteps);

    // Retrieve a single step with id
    app.get("/workouts/:workoutId/steps/:position", steps.findOneStep);

    // retreive id of first step
    app.get("/workouts/:workoutId/firstStep", steps.findFirst);
};