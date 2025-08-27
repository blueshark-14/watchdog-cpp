# 🛡️ Watchdog-CPP

A modern, cross-platform-inspired **process monitoring and management tool** written in C++ for Windows.  
This project demonstrates advanced Windows API usage, robust C++11 code structure, and practical system programming skills.

---

## 🚀 Features

- **Live Process Monitoring:**  
  Reads a configurable list of processes from `config.json` and monitors their status in real time.

- **Interactive Console Control:**  
  Easily check, start, kill, or bring processes to the foreground via a user-friendly menu.

- **Windows API Integration:**  
  Uses native WinAPI for process enumeration, launching, termination, and window management.

- **Configurable:**  
  All monitored processes and the preferred foreground app are defined in a simple JSON file.

- **Clean, Modular C++ Design:**  
  Follows best practices with clear separation of concerns (`ConfigManager`, `ProcessMonitor`, `OSApiWrapper`, `ProcessInfo`).

---

## 📂 Project Structure

```
watchdog-cpp/
├── src/
│   ├── main.cpp
│   ├── ConfigManager.h/cpp
│   ├── ProcessMonitor.h/cpp
│   ├── OSApiWrapper.h/cpp
│   └── ProcessInfo.h
├── config.json
└── .vscode/
    └── tasks.json
```

---

## 📝 Example `config.json`

```json
{
  "processes": [
    { "name": "notepad.exe", "args": "" },
    { "name": "calc.exe", "args": "" }
  ],
  "foreground": "notepad.exe"
}
```

---

## 💻 Usage

1. **Build the project**  
   - Open in VS Code and press <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>B</kbd> (uses `.vscode/tasks.json`).

2. **Run the executable**  
   - From the terminal:  
     ```
     build\main.exe
     ```

3. **Interact via the menu**  
   - Choose to check, start, kill, or bring a process to the foreground.
   - Example output:
     ```
     Choose an action:
     1. Check if process is running
     2. Start process
     3. Bring process to foreground
     4. Kill process
     5. Exit
     Enter your choice:
     ```

---

## 🛠️ Technical Highlights

- **C++11**: Uses modern C++ features for safety and clarity.
- **Windows API**: Directly interacts with system processes and windows.
- **Robust Error Handling**: Gracefully handles missing processes and invalid user input.
- **Extensible**: Easily add more process management features or port to other platforms.

---

## 📸 Screenshots

<img width="1169" height="996" alt="Image" src="https://github.com/user-attachments/assets/7d154a47-43b5-46c2-843d-22ed1a1c6c7c" />
*Sample interactive session monitoring Notepad and Calculator.*

---

## 👨‍💻 Why This Project?

- **Demonstrates real-world C++ system programming**
- **Showcases Windows API proficiency**
- **Highlights clean, maintainable code and modular design**
- **Ready for extension or integration into larger monitoring solutions**

---

## 📢 Recruiter Notes

- **Code is clean, well-commented, and production-ready.**
- **Demonstrates both low-level Windows API skills and modern C++ design.**
- **Shows ability to build practical, user-facing developer tools.**

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

> **Developed with care by Rakib Hasan**  
> _Feel free to explore, use, and extend!_
