const db = require("../models");
const Step = db.steps;

// create and save a new step 
exports.createStep = (req, res) => {

};

// Retrieve all steps from the database.
exports.findAllSteps = (req, res) => {

};

// Find a single step with an id
exports.findOneStep = (req, res) => {

};

exports.findFirst = (req, res) => {

}

// create a new step
exports.createStep = (req, res) => {
    // Validate request
    if (!req.body) {
        res.status(400).send({
            message: "Content can not be empty!"
        });
    }

    // Create a step
    const step = {
        side: req.body.side,
        val: req.body.val,
        time: req.body.time,
        position: req.body.position,
        currentStep: req.body.currentStep,
        workoutId: req.params.workoutId
    };

    // save step in the database
    Step.create(step)
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message:
                    err.message || "Error creating Step."
            });
        });
};

// Retreive all steps from database
exports.findAllSteps = (req, res) => {
    Step.findAll({
        where: { workoutId: req.params.workoutId }
    })
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message:
                    err.message || "Error retrieving step"
            });
        });
};

// Retreive a single step from databse
exports.findOneStep = (req, res) => {
    const workoutId = req.params.workoutId;
    const position = req.params.position;

    Step.findAll({
        where: {
            workoutId: workoutId, position: position 
        }
    })
        .then(data => {
            if (data.length) {
                res.send(data);
            }
            else {
                res.status(400).send({
                    message: "Step with id: " + id + " in workout: " + workoutId + " does not exist"
                })
            }
        })
        .catch(err => {
            res.status(500).send({
                message:
                    err.message || "Error retrieving step"
            });
        });
};


// Retreive Id of first step 
exports.findFirst = (req, res) => {
    Step.findOne({
        where: { workoutId: req.params.workoutId }
    })
        .then(data => {
            res.send(data);
        })
        .catch(err => {
            res.status(500).send({
                message:
                    err.message || "Error retreiving first step"
            });
        });
}
