#include <algorithm>
#include <iostream>
#include <string>
#include <limits>
#include <thread>
#include <atomic>
#include "ConfigManager.h"
#include "OSApiWrapper.h"
#include "ProcessMonitor.h"
using namespace std;

int main() {
    ConfigManager cfg("config.json");
    OSApiWrapper api;
    ProcessMonitor monitor(cfg, api);

    // Run the monitor in a background thread so the user can interact with the menu
    std::atomic<bool> running{true};
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
}