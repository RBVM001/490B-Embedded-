module.exports = (sequelize, DataTypes) => {
    const User = sequelize.define("user", {
		
		name: {
			type: DataTypes.STRING(500),
			allowNull:false
		},
		email: {
			type: DataTypes.STRING(500),
			allowNull: false
		},
		userName: {
			type: DataTypes.STRING(500),
			allowNull: false
		},
		password: {
			type: DataTypes.STRING(500),
			allowNull: false
		},
		weight: {
			type: DataTypes.INTEGER,
			allowNull: false
		},
		height: {
			type: DataTypes.INTEGER,
			allowNull: false
		},
		age : {
			type: DataTypes.INTEGER,
			allowNull: true
		},
    },
    {
        timestamps: true,
        createdAt: true,
        updatedAt: true,
    });
    return User;
};