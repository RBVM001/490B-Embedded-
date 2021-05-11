const express = require('express');
const bodyParser = require("body-parser");;

const app = express();

// parse requests of content-type: app/json
app.use(bodyParser.json());

// parse requests of content-type: app/x-www-form-urlencoded
app.use(bodyParser.urlencoded({extended: true}));

// simple route
app.get("/", (req,res) => {
	res.json({message: "welcome to Solidarity app."});
});

const db = require("./app/models");
const run = async () => {

};
db.sequelize.sync({ force: false }).then(() => {
	run();
});
db.sequelize.sync();
require("./app/routes/routes")(app);
// set port, listen for requests
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}.`);
});
