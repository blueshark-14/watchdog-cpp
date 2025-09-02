# 🛡️ Watchdog-CPP

A modern, cross-platform-inspired **process monitoring and management tool** written in C++ for Windows and Linux.  
This project demonstrates advanced Windows and Linux API usage, robust C++11 code structure, and practical system programming skills.

---

## 🚀 Features

- **Live Process Monitoring:**  
  Reads a configurable list of processes from `config.json` and monitors their status in real time.

- **Interactive Console Control:**  
  Easily check, start, kill, or bring processes to the foreground via a user-friendly menu.

- **Native OS API Integration:**  
  Uses a dedicated OS API component for process enumeration, launching, termination, and window management.  
  - **Windows:** Uses native WinAPI  
  - **Linux:** Uses POSIX/system calls

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
│   ├── WindowsApiWrapper.h/cpp
│   ├── LinuxApiWrapper.h/cpp
│   └── ProcessInfo.h
├── tests/
│   └── unit/
│       ├── test_ConfigManager.cpp
│       ├── test_ProcessMonitor.cpp
│       └── catch.hpp
├── config.json
├── .github/
│   └── workflows/
│       └── ci.yml
└── .vscode/
    └── tasks.json
```

---

## 🌐 Cross-Platform Architecture

- **OS API Isolation:**  
  All platform-specific logic is encapsulated in the `OSApiWrapper` interface and its implementations:  
  - `WindowsApiWrapper` for Windows (WinAPI)
  - `LinuxApiWrapper` for Linux (POSIX/system calls)

- **Single Codebase:**  
  The main application logic (`ProcessMonitor`, `ConfigManager`, etc.) is OS-agnostic and interacts only with the `OSApiWrapper` interface.

- **Platform Selection:**  
  The correct OS API implementation is selected at compile time using preprocessor macros:
  ```cpp
  #ifdef _WIN32
      WindowsApiWrapper api;
  #else
      LinuxApiWrapper api;
  #endif
  ```

---

## 🏗️ Building for Windows and Linux

- **Windows Build:**
  ```sh
  g++ -std=c++11 -Isrc src/main.cpp src/ConfigManager.cpp src/ProcessMonitor.cpp src/OSApiWrapper.cpp src/WindowsApiWrapper.cpp -o build/main.exe
  ```

- **Linux Build:**
  ```sh
  g++ -std=c++11 -Isrc src/main.cpp src/ConfigManager.cpp src/ProcessMonitor.cpp src/OSApiWrapper.cpp src/LinuxApiWrapper.cpp -o build/main
  ```
  *(Add `-lstdc++fs` if your g++ version requires it for `<filesystem>`)*

> **Note:**  
> The Linux build requires C++17 or newer because `LinuxApiWrapper` uses `std::filesystem`.  
> Use `-std=c++17` (or newer) for Linux builds:
> ```
> g++ -std=c++17 -Isrc src/main.cpp src/ConfigManager.cpp src/ProcessMonitor.cpp src/OSApiWrapper.cpp src/LinuxApiWrapper.cpp -o build/main
> ```
> If you get a linker error about filesystem, add `-lstdc++fs` (needed for GCC 8 and earlier):
> ```
> g++ -std=c++17 ... -lstdc++fs
> ```

- **Tip:**  
  For larger projects, consider using [CMake](https://cmake.org/) to manage platform-specific sources and build configurations.

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
     - On Windows:  
       ```
       build\main.exe
       ```
     - On Linux:  
       ```
       ./build/main
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
- **Windows & Linux API**: Directly interacts with system processes and windows (WinAPI/POSIX).
- **Robust Error Handling**: Gracefully handles missing processes and invalid user input.
- **Extensible**: Easily add more process management features or port to other platforms.

---

## 📸 Screenshots

<img width="1169" height="996" alt="Image" src="https://github.com/user-attachments/assets/7d154a47-43b5-46c2-843d-22ed1a1c6c7c" />
*Sample interactive session monitoring Notepad and Calculator.*

---

## 👨‍💻 Why This Project?

- **Demonstrates real-world C++ system programming**
- **Showcases Windows and Linux API proficiency**
- **Highlights clean, maintainable code and modular design**
- **Ready for extension or integration into larger monitoring solutions**

---

## 📢 Recruiter Notes

- **Code is clean, well-commented, and production-ready.**
- **Demonstrates both low-level Windows/Linux API skills and modern C++ design.**
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
| Integration tests for workflows                                  | ✅ Done   |
| Unit tests for all component methods                             | ✅ Done   |
| **Full watchdog workflow for both Linux and Windows**            | ✅ Done   |
| **Separate build configurations for Linux and Windows**          | ✅ Done   |

---

## 🧪 About Catch2 Unit Testing Macros

- **Catch2** is a modern C++ unit testing framework used in this project.
- **`TEST_CASE`** defines a test function; each is run independently.
- **`REQUIRE(expr)`** asserts that `expr` is true; if not, the test fails immediately and reports the error.
- **`CHECK(expr)`** asserts that `expr` is true; if not, the test logs a failure but continues running the test.
- These macros make writing and reading tests simple, expressive, and reliable.

---

## 🧪 Running Unit Tests

1. **Build the unit tests**  
   - Open a terminal in your project root.
   - Example for `test_ConfigManager.cpp`:
     ```
     g++ -std=c++11 -Isrc -Itests/unit tests/unit/test_ConfigManager.cpp src/ConfigManager.cpp -o tests/unit/test_ConfigManager.exe
     ```
     - `-Isrc` tells the compiler to look for header files in the `src` directory (so `#include "ConfigManager.h"` works).
     - `-Itests/unit` tells the compiler to look for headers (like `catch.hpp`) in the `tests/unit` directory.
   - Example for `test_ProcessMonitor.cpp`:
     ```
     g++ -std=c++11 -Isrc -Itests/unit tests/unit/test_ProcessMonitor.cpp src/ProcessMonitor.cpp src/ConfigManager.cpp -o tests/unit/test_ProcessMonitor.exe
     ```
     - Add any other `.cpp` files your test depends on.

2. **Run the unit test executables**  
   ```
   tests/unit/test_ConfigManager.exe
   tests/unit/test_ProcessMonitor.exe
   ```

- All test results and assertion details will be shown in the terminal.

---

## 🧪 Unit Testing and OOP Concepts

- Unit tests for `ProcessMonitor` use mock classes (`MockApi`, `MockConfig`) to simulate dependencies.
- **OOP concepts applied:**
  - **Abstraction & Polymorphism:** Achieved by declaring methods in `OSApiWrapper` as `virtual = 0` (pure virtual) and using `override` in mocks.
  - **Encapsulation:** Each component (API, config, monitor) is tested in isolation.
  - **Testability:** Mocks allow testing logic without real OS/process interaction.
- **OOP concepts in MockApi:**
  - Used `virtual = 0` (pure virtual) in the base class to enable abstraction and polymorphism.
  - Used `override` in `MockApi` to ensure correct method overriding and safe mocking for unit tests.

---

## 🚦 Continuous Integration

This project uses [GitHub Actions](https://github.com/features/actions) for automated building and unit testing.
- On every push or pull request, all unit tests are built and run automatically.
- See `.github/workflows/ci.yml` for details.
- You can view the status and results for each commit or PR in the **Actions** tab on GitHub.

---

## ⚠️ Limitations

- Some integration and UI tests may still require manual verification.
- Automated unit tests cover core logic and components, but not all possible edge cases or real-world scenarios.
- On Linux, `bringToForeground` and `isProcessInForeground` are not implemented due to platform limitations.

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---

> **Developed with care by Rakib Hasan**  
> _Feel free to explore, use, and extend!_