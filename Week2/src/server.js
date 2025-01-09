const express = require('express');
const bodyParser = require('body-parser');
//const mongoose = require('mongoose');
require('dotenv').config();
const connectToDatabase = require('./config/db');
const userRoutes = require('../src/routes/users');
const taskRoutes = require('../src/routes/tasks');
const errorHandler = require('../src/middleware/errorHandler'); // Import error handler
const auth = require('../src/middleware/auth');

const app = express();
const port = process.env.PORT || 3000;

app.use(bodyParser.json());

connectToDatabase.connectToDatabase(); 

app.use('/users', userRoutes);
app.use('/tasks', auth, taskRoutes); // Apply auth middleware to all task routes

app.use(errorHandler); // Apply error handling middleware

app.listen(port, () => {
    console.log(`Server listening on port ${port}`);
});