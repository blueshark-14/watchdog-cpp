#include <algorithm>
#include <iostream>
#include <string>
#include <limits>
#include "ConfigManager.h"
#include "OSApiWrapper.h"
#include "ProcessMonitor.h"
using namespace std;

int main() {
    ConfigManager cfg("config.json");
    cfg.load();

    OSApiWrapper api;
    ProcessMonitor monitor(cfg, api);

    monitor.run();

    std::string procName = "notepad.exe";
    int choice = 0;

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

       switch (choice) {
            case 1: {
                bool running = api.isProcessRunning(procName);
                std::cout << procName << " running? " << (running ? "Yes" : "No") << std::endl;
                break;
            }
            case 2:
                api.startProcess(procName, "");
                break;
            case 3:
                api.bringToForeground(procName);
                break;
            case 4: {
                bool running = api.isProcessRunning(procName);
                if (running) {
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

    std::cout << "Foreground app should be: " << cfg.getForegroundApp() << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get(); // wait for Enter
}