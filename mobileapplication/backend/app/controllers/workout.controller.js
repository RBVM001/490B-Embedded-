const db = require("../models");
const Workout = db.workouts;

// create and save a new workout 
exports.createWorkout = (req, res) => {

};

// Retrieve all workouts from the database.
exports.findAllWorkouts = (req, res) => {

};

// Find a single workout with an id
exports.findOneWorkout = (req, res) => {

};

// Update a workout by the id in the request
exports.updateWorkout = (req, res) => {

};

// Delete a workout with the specified id in the request
exports.deleteWorkout = (req, res) => {

};

// Delete all workouts from the database.
exports.deleteAllWorkouts = (req, res) => {

};

// create a new workout
exports.createWorkout = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            message: "Content can not be empty!"
        });
    }

    // Create a workout
	const workout = {
		userId: req.params.userId,
        date_created: req.body.date_created,
        elapsed_time: req.body.elapsed_time,
    };

    // save workout in the database
    Workout.create(workout)
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message:
                    err.message || "Error creating Workout."
            });
        });
};

exports.findAllWorkouts = (req, res) => {
	const userId = rep.params.userId;
	Workout.findAll({ where: { userId: userId } })
	.then(data => {
			res.send(data);
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error retrieving workout"
			});
		});
};

exports.findOneWorkout = (req, res) => {
	const id = req.params.id;

	Workout.findAll({ where: { id: id } })
		.then(data => {
			res.send(data);
		})
		.catch(err => {
			res.status(500).send({
				message: "Error retrieving workout with id =" + id
			});
		});
};

exports.updateWorkout = (req, res) => {
	const id = req.params.id;

	Workout.update(req.body, {
		where: { id: id }
	})
		.then(num => {
			if (num == 1) {
				res.send({
					message: "Workout was updated succesfully"
				});
			} else {
				res.send({
					message: 'Can NOT update wokrout with id=${id}.'
				})
			}
		})
		.catch(err => {
			res.status(500).send({
				message: "Error updating workout"
			});
		});
};

exports.deleteWorkout = (req, res) => {
	const id = req.params.id;

	Workout.destroy({
		where: { id: id }
	})
		.then(num => {
			if (num == 1) {
				res.send({
					message: "Workout was deleted succesfully"
				});
			} else {
				res.send({
					message: "can NOT delete workout"
				});
			}
		})
		.catch (err => {
			res.status(500).send({
				message: "Could not delete workout with id=" + id
			});
		});
};

exports.deleteAllWorkouts = (req, res) => {
	Workout.destroy({
		where: {},
		truncate: false
	})
		.then(nums => {
			res.send({
				message: '${nums} workouts were deleted succesfully'
			});
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error removing all workouts"
			});
		});
};

