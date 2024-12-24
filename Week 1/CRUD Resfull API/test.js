const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 3000;

// Middleware for parsing request bodies
app.use(bodyParser.json());

// Sample data (replace with a database)
let tasks = [
//   { id: 1, title: 'Task 1', description: 'Description for Task 1', completed: false },
//   { id: 2, title: 'Task 2', description: 'Description for Task 2', completed: true }
];

// Generate unique IDs for new tasks
let nextId = 1;

// Middleware for request validation
const validateTask = (req, res, next) => {
  const { title, description } = req.body;
  if (!title || !description) {
    return res.status(400).json({ error: 'Title and description are required' });
  }
  next();
};

// GET /tasks - Get all tasks
app.get('/tasks', (req, res) => {
  res.json(tasks);
});

// GET /tasks/:id - Get a single task
app.get('/tasks/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const task = tasks.find(t => t.id === id);
  if (task) {
    res.json(task);
  } else {
    res.status(404).json({ error: 'Task not found' });
  }
});

// POST /tasks - Create a new task
app.post('/tasks', validateTask, (req, res) => {
  const newTask = {
    id: nextId++,
    title: req.body.title,
    description: req.body.description,
    completed: false
  };
  tasks.push(newTask);
  res.status(201).json(newTask);
});

// PUT /tasks/:id - Update an existing task
app.put('/tasks/:id', validateTask, (req, res) => {
  const id = parseInt(req.params.id);
  const taskIndex = tasks.findIndex(t => t.id === id);
  if (taskIndex !== -1) {
    //merges the entire new req body in the old body
    //tasks[taskIndex].body = req.body;
    
    //Assuming title, description and completed are there in the new request
    // tasks[taskIndex].title = req.body.title;
    // tasks[taskIndex].description = req.body.description;
    // tasks[taskIndex].completed=req.body.completed;

    tasks[taskIndex] = { ...tasks[taskIndex], ...req.body };
    res.json(tasks[taskIndex]);
  } else {
    res.status(404).json({ error: 'Task not found' });
  }
});

// DELETE /tasks/:id - Delete a task
app.delete('/tasks/:id', (req, res) => {
  const id = parseInt(req.params.id);
  const taskIndex = tasks.findIndex(t => t.id === id);
  if (taskIndex !== -1) {
    tasks.splice(taskIndex, 1);
    res.sendStatus(204); // No content
    nextId--;
  } else {
    res.status(404).json({ error: 'Task not found' });
  }
});

app.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});