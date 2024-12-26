const mongoose = require('mongoose');
require('dotenv').config(); // Import dotenv

const connectToDatabase = async () => {
  try {
    //const uri = "mongodb+srv://akritisonker44:BsVi1CQuwjXoMIOM@cluster1.4r9j2.mongodb.net/?retryWrites=true&w=majority&appName=CLUSTER1"
    
    //const uri = process.env.MONGODB_URI;
    console.log(process.env.MONGODB_URI); 
    await mongoose.connect(process.env.MONGODB_URI, { useNewUrlParser: true, useUnifiedTopology: true });
    console.log('Connected to MongoDB');
  } catch (error) {
    console.error('Error connecting to MongoDB:', error);
    //process.exit(1); // Exit the application if connection fails
  }
};

const user = require('../model/schema');

module.exports = { connectToDatabase };