Execution Documentation for Node.js To-Do Application

This document outlines the execution steps and functionalities of the provided Node.js application for managing tasks.

Prerequisites:
•	Node.js and npm installed on your system.
•	A code editor or IDE for editing JavaScript code. (VS Code)

Setting Up the Project:
  
1.	Clone the repository.
2.	Install dependencies: run ‘npm install’ to install the required dependencies listed in the ‘package.json’ file.
   
Database Used (MongoDB)

Running the Application:
Start the server by node server.js in the terminal inside the /Week1/src folder. This will start the Node.js on the specified port (default: 3000).


4. API Endpoints:
   
The application provides several API endpoints for managing tasks:

•	GET /tasks: Retrieves a list of all tasks stored in the database.

•	GET /tasks/:id: Retrieves a single task by its ID.

•	POST /tasks: Creates a new task. The request body should contain title and description properties for the new task. (middleware validation)

•	PUT /tasks/:id: Updates an existing task by its ID. The request body should contain the updated properties for the task.

•	DELETE /tasks/:id: Deletes a task by its ID.

By following these steps and understanding the functionalities, you can effectively use this Node.js application to manage your tasks through API requests.
