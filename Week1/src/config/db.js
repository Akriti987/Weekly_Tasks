const mongoose = require('mongoose');
require('dotenv').config(); // Import dotenv

const connectToDatabase = async () => {
  try {
    //const uri = process.env.MONGODB_URI;
    //console.log(process.env.MONGODB_URI); 
    await mongoose.connect(process.env.MONGODB_URI, { useNewUrlParser: true, useUnifiedTopology: true });
    console.log('Connected to MongoDB');
  } catch (error) {
    console.error('Error connecting to MongoDB:', error);
    //process.exit(1); // Exit the application if connection fails
  }
};


module.exports = { connectToDatabase };