const express = require('express');
const bodyParser = require('body-parser');
require('dotenv').config();
const connectToDatabase = require('./config/db.js'); // Import database connection 
const Task = require('./model/schema.js'); // Import Task model
const app = express();

const port = process.env.PORT || 3000;
connectToDatabase.connectToDatabase();

app.use(bodyParser.json());

//let tasks = [
//   { id: 1, title: 'Task 1', description: 'Description for Task 1', completed: false }
//];


// Middleware for request validation
const validateTask = (req, res, next) => {
  const { title, description } = req.body;
  if (!title || !description) {
    return res.status(400).json({ error: 'Title and description are required' });
  }
  next();
};

// GET /tasks - Get all tasks
app.get('/tasks', async (req, res) => {
  const tasks = await Task.find();
  res.json(tasks);
});

// GET /tasks/:id - Get a single task
app.get('/tasks/:id', async (req, res) => {
  // const id = parseInt(req.params.id);
  // const task = tasks.find(t => t.id === id);
  // if (task) {
  //   res.json(task);
  // } else {
  //   res.status(404).json({ error: 'Task not found' });
  // }
  try{
  const task = await Task.findById(req.params.id); 
    if (!task) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.json(task);
  }
  catch(error){
    console.error(error);
    res.status(400).json({ error: 'Bad Request' });
  }
});

// POST /tasks - Create a new task
app.post('/tasks', validateTask, async (req, res) => {
  // const newTask = {
  //   id: nextId++,
  //   title: req.body.title,
  //   description: req.body.description,
  //   completed: false
  // };
  // tasks.push(newTask);
  // res.status(201).json(newTask);
  const newTask = new Task(req.body); 
    const savedTask = await newTask.save(); 
    res.status(201).json({ message: 'Task created successfully', task: savedTask });
});

// PUT /tasks/:id - Update an existing task
app.put('/tasks/:id', validateTask, async (req, res) => {
    
    // tasks[taskIndex].title = req.body.title;
    // tasks[taskIndex].description = req.body.description;
    // tasks[taskIndex].completed=req.body.completed;

  //   tasks[taskIndex] = { ...tasks[taskIndex], ...req.body };
  //   res.json(tasks[taskIndex]);
  // } else {
  //   res.status(404).json({ error: 'Task not found' });
  // }

  const updatedTask = await Task.findByIdAndUpdate(req.params.id, req.body, { new: true }); 
    if (!updatedTask) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.json(updatedTask);

});

// DELETE /tasks/:id - Delete a task
app.delete('/tasks/:id', async (req, res) => {
  // const id = parseInt(req.params.id);
  // const taskIndex = tasks.findIndex(t => t.id === id);
  // if (taskIndex !== -1) {
  //   tasks.splice(taskIndex, 1);
  //   res.sendStatus(204); // No content
  //   nextId--;
  // } else {
  //   res.status(404).json({ error: 'Task not found' });
  // }
  const result = await Task.findByIdAndDelete(req.params.id); 
    if (!result) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.status(204).json();
  
});

app.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});