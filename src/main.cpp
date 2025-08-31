#include <algorithm>
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <atomic>
#include "ConfigManager.h"
#include "WindowsApiWrapper.h"
#include "ProcessMonitor.h"
#include "Logger.h"
using namespace std;

int main() {
    ConfigManager cfg("config.json");
    WindowsApiWrapper api;
    ProcessMonitor monitor(cfg, api);

     // Run the monitor in the main thread (no user menu)
    monitor.run([]() { return true; }); // This will run indefinitely

    // Run the monitor in a background thread so the user can interact with the menu
   // As no user interaction is needed, we can comment out it now
    /*  std::atomic<bool> running{true};
    std::thread monitorThread([&]() {
        monitor.run([&running]() { return running.load(); });
    });

    int choice = 0;
    std::string procName, args;

    while (true) {
        std::cout << "\nChoose an action:\n";
        std::cout << "1. Check if process is running\n";
        std::cout << "2. Start process\n";
        std::cout << "3. Bring process to foreground\n";
        std::cout << "4. Kill process\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a number between 1 and 5.\n";
            continue; // restart loop
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer
        if (choice == 5) break;

        std::cout << "Enter process name (e.g., notepad.exe): ";
        std::getline(std::cin, procName);

        switch (choice) {
            case 1: {
                bool runningProc = api.isProcessRunning(procName);
                std::cout << procName << " running? " << (runningProc ? "Yes" : "No") << std::endl;
                break;
            }
            case 2: {
                std::cout << "Enter arguments (or leave blank): ";
                std::getline(std::cin, args);
                api.startProcess(procName, args);
                break;
            }
            case 3:
                api.bringToForeground(procName);
                break;
            case 4: {
                bool runningProc = api.isProcessRunning(procName);
                if (runningProc) {
                    api.killProcess(procName);
                } else {
                    std::cout << procName << " is not running. Nothing to kill.\n";
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    }

    // Stop the monitor thread
    running = false;
    if (monitorThread.joinable()) monitorThread.join();

    std::cout << "Foreground app should be: " << cfg.getForegroundApp() << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get(); // wait for Enter
    */
}