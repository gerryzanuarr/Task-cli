# 📋 task-cli – Terminal Task Manager in C++

`task-cli` is a simple, terminal-based task manager written entirely in standard C++ without any external libraries. All task data is saved in a local `task.json` file using a manually structured JSON format.

---

## ✨ Features

- Add new tasks with unique IDs
- Update task status: `todo`, `in-progress`, or `done`
- Delete tasks by ID
- View all tasks
- Filter tasks by status
- Track task creation and last update times (`createdAt`, `updatedAt`)
- Stores all data in a plain `task.json` file

---

## 📦 Data Format (`task.json`)

Here's an example of how tasks are stored in the JSON file:

```json
[
  {
    "id": 1,
    "description": "Learn C++",
    "status": "todo",
    "createdAt": "2025-06-21 10:00:00",
    "updatedAt": "2025-06-21 10:00:00"
  }
]


---

🛠️ How to Use

1. Compile the Program

Make sure you have g++ installed, then run:

g++ -o task-cli task-cli.cpp

2. Run the Program

./task-cli

> The program will automatically create task.json if it doesn't already exist.




---

📚 Menu Overview

When running, you'll see this menu:

--- Menu ---
1. Add task
2. Update task status
3. Delete task
4. View all tasks
5. View completed tasks (done)
6. View pending tasks (todo)
7. View tasks in progress (in-progress)
0. Exit

When updating task status, choose from:

1. todo
2. in-progress
3. done


---

💻 Compatibility

✅ Linux, macOS, and Windows (terminal/command-line)

✅ Works offline

✅ Requires only standard C++ (no external libraries)



---

📄 License

This project is free to use and modify for personal or educational purposes.

---

Project URL
https://roadmap.sh/projects/task-tracker
