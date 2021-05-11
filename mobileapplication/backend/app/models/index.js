const dbConfig = require("../config/db.config.js");

const Sequelize = require("sequelize");
const sequelize = new Sequelize(dbConfig.DB, dbConfig.USER, dbConfig.PASSWORD, { host: dbConfig.HOST, dialect: dbConfig.dialect});

const db = {};

db.Sequelize = Sequelize;
db.sequelize = sequelize;

db.users = require("./user.model.js")(sequelize, Sequelize);
db.workouts = require("./workout.model.js")(sequelize, Sequelize);
db.steps = require("./step.model.js")(sequelize, Sequelize);

db.workouts.hasMany(db.steps, {
	as: "step",
	onDelete: 'cascade',
	onUpdate: 'no action',
	hooks: true});
db.steps.belongsTo(db.workouts, {
	foreignKey: "workoutId",
	as: "workout"
});

db.users.hasMany(db.workouts, {
	as: "workout",
	onDelete: 'cascade',
	onUpdate: 'no action',
	hooks: true
});
db.workouts.belongsTo(db.users, {
	foreignKey: "userId",
	as: "user"
});
module.exports = db; 