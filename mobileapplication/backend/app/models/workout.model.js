module.exports = (sequelize, DataTypes) => {
    const Workout = sequelize.define("workout", {
        elapsed_time: {
            type: DataTypes.TIME,
            allowNull: true
        },
        stepCount: {
            type: DataTypes.INTEGER,
            allowNull: true
        },
        distance: {
            type: DataTypes.INTEGER,
            allowNull: true
        },
        calories: {
            type: DataTypes.INTEGER,
            allowNull: true
        },
    },
    {
        timestamps: true,
        createdAt: true,
        updatedAt: true,
    });
    return Workout;
};