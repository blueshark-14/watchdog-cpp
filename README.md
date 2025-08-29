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

## 📝 Note on Monitoring Multi-Process Applications (e.g., chrome.exe)

**Some applications, like Google Chrome, use a multi-process architecture:**
- When you launch Chrome (`chrome.exe`), it starts a main process and then spawns multiple child processes for tabs, extensions, and other features.
- All these processes typically have the same executable name (`chrome.exe`).

**Implications for monitoring:**
- The watchdog checks if any process with the name `chrome.exe` is running.
- If at least one `chrome.exe` process exists, the watchdog considers Chrome as "running" and will not start a new instance.
- If you close all Chrome windows, all `chrome.exe` processes will terminate, and the watchdog will restart Chrome.

**Best Practice:**
- Use only the executable name (`chrome.exe`) in your config, not the full path, to ensure reliable detection.
- Example:
  ```json
  { "name": "chrome.exe", "args": "" }
  ```

**Summary:**  
For multi-process applications like Chrome, always monitor by executable name, not full path, to prevent repeated launches and ensure correct process management.

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

## ✅ Requirements Coverage

| Requirement                                                      | Status    |
|------------------------------------------------------------------|-----------|
| Config file on startup                                           | ✅ Done   |
| Dynamic config reload                                            | ✅ Done   |
| List of processes & startup parameters in config                 | ✅ Done   |
| Check/start all on startup                                       | ✅ Done   |
| Dynamic add/start new process                                    | ✅ Done   |
| Restart if stopped/killed                                        | ✅ Done   |
| Stop monitoring removed processes                                | ✅ Done   |
| Periodic process monitoring                                      | ✅ Done   |
| Native OS API as separate component                              | ✅ Done   |
| Extensible for other OS                                          | ✅ Done   |
| Log all events to Windows Event Log                              | ✅ Done   |
| Integration tests for workflows                                  | ⬜ Pending|
| Unit tests for all component methods                             | ⬜ Pending|

> **Note:** Integration and unit tests are planned for future updates.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

> **Developed with care by Rakib Hasan**  
> _Feel free to explore, use, and extend!_
