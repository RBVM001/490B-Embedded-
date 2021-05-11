module.exports = (sequelize, DataTypes) => {
	const Step = sequelize.define("step", {
		side: {
			type: DataTypes.CHAR,
			allowNull: false
		},
		val: {
			type: DataTypes.STRING(2000),
			allowNull: false
		},
		currentStep: {
			type: DataTypes.INTEGER,
			allowNull: false
		},
		time: {
			type: DataTypes.TIME,
			allowNull: false
		},
		position: {
			type: DataTypes.INTEGER,
			allowNull: false
		},
	},
		{
			timestamps: true,
			createdAt: true,
			updatedAt: false,
	});
	return Step;
};
