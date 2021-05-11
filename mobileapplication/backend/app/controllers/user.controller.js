const db = require("../models");
const User = db.users;

// create and save a new user 
exports.createUser = (req, res) => {

};

// create and save a new user 
exports.loginUser = (req, res) => {

};

// Retrieve all users from the database.
exports.findAllUsers = (req, res) => {

};

// Find a single user with an id
exports.findOneUser = (req, res) => {

};

// Update a user by the id in the request
exports.updateUser = (req, res) => {

};

// Delete a user with the specified id in the request
exports.deleteUser = (req, res) => {

};

// Delete all users from the database.
exports.deleteAllusers = (req, res) => {

};


// create a new user
exports.createUser = (req, res) => {
	// Validate request
	if (!req.body) {
		res.status(400).send({
			message: "Content can not be empty!"
		});
	}

	// Create a user
	const user = {
		name: req.body.name,
		email: req.body.email,
		userName: req.body.userName,
		password: req.body.password,
		weight: req.body.weight,
		height: req.body.height
	};
	// check if user with email exists
	User.findAll({
		where: {
			email: req.body.email
		}
	})
		.then(data => {
			if (data.length) {
				res.status(400).send({
					message: "User with email: " + user.email + " EXISTS"
				});
			}
			else {
				// save user in the database
				User.create(user)
					.then(data => {
						res.status(200).send(data);
					})
					.catch(err => {
						res.status(500).send({
							message:
								err.message || "Error creating User."
						});
					});
			};
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error looking for user"
			});
		});
};

exports.loginUser = (req, res) => {
	const email = req.body.email;
	const password = req.body.password;

	User.findAll({
		where: {
			email: email, password: password
		}
	})
		.then(data => {
			if (data.length) {
				res.status(200).send(data);
			}
			else {
				res.status(404).send({
					message: "Login failed"
				})
			}
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error retrieving user"
			});
		});
};

exports.findAllUsers = (req, res) => {
	User.findAll()
		.then(data => {
			res.send(data);
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error retrieving user"
			});
		});
};

exports.findOneUser = (req, res) => {
	const id = req.params.id;

	User.findAll({ where: { id: id } })
		.then(data => {
			res.send(data);
		})
		.catch(err => {
			res.status(500).send({
				message: "Error retrieving user with ID =" + id
			});
		});
};

exports.updateUser = (req, res) => {
	const id = req.params.id;

	User.update(req.body, {
		where: { id: id }
	})
		.then(num => {
			if (num == 1) {
				res.send({
					message: "User was updated succesfully"
				});
			} else {
				res.send({
					message: 'Can NOT update user with id=${id}.'
				})
			}
		})
		.catch(err => {
			res.status(500).send({
				message: "Error updating user"
			});
		});
};

exports.deleteUser = (req, res) => {
	const id = req.params.id;

	User.destroy({
		where: { id: id }
	})
		.then(num => {
			if (num == 1) {
				res.send({
					message: "User was deleted succesfully"
				});
			} else {
				res.send({
					message: "can NOT delete user"
				});
			}
		})
		.catch (err => {
			res.status(500).send({
				message: "Could not delete user with id=" + id
			});
		});
};

exports.deleteAllUsers = (req, res) => {
	User.destroy({
		where: {},
		truncate: false
	})
		.then(nums => {
			res.send({
				message: '${nums} users were deleted succesfully'
			});
		})
		.catch(err => {
			res.status(500).send({
				message:
					err.message || "Error removing all users"
			});
		});
};

