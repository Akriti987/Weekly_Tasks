const Task = require('../model/schema');

async function checkForDuplicate(title ,description,req) {
  const exsistingTask = await Task.findOne({title , description, user: req.user._id});
  return (exsistingTask!=null)
  }

const createTask = async (req, res) => {
  try {
  const { title, description } = req.body;
  const isDuplicate = await checkForDuplicate(title, description,req);
  //console.log(isDuplicate);
  if(isDuplicate)
  return res.status(409).json({message:"Task already exists"});
  const newTask = new Task({ title, description, user: req.user._id }); 
  const savedTask = await newTask.save(); 
  res.status(201).json({ message: 'Task created successfully', task: savedTask });
  
}
  catch (error) {
    console.error(error);
    res.status(500).json({ error: 'Internal server error' });
}
};

const getTasks = async (req, res) => {
  try {
    const tasks = await Task.find({ user: req.user._id }); 
    res.json(tasks);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: 'Internal server error' });
  }
};

const getTask = async (req, res) => {
  try {
    const task = await Task.findOne({ _id: req.params.id, user: req.user._id }); 
    if (!task) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.json(task);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: 'Internal server error' });
  }
};

const updateTask = async (req, res) => {
  try {
    const updatedTask = await Task.findOneAndUpdate(
      { _id: req.params.id, user: req.user._id }, 
      req.body, 
      { new: true }
    );
    if (!updatedTask) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.json(updatedTask);
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: 'Internal server error' });
  }
};

const deleteTask = async (req, res) => {
  try {
    const result = await Task.findOneAndDelete({ _id: req.params.id, user: req.user._id });
    if (!result) {
      return res.status(404).json({ error: 'Task not found' });
    }
    res.sendStatus(204); 
  } catch (error) {
    console.error(error);
    res.status(500).json({ error: 'Internal server error' });
  }
};

module.exports = {
  createTask,
  getTasks,
  getTask,
  updateTask,
  deleteTask
};