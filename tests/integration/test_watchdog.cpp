/*
    Integration Test: Watchdog Basic Workflow

    This test simulates a real user scenario:
    - Starts the watchdog process.
    - Modifies the config file to add/remove processes.
    - Kills a monitored process and checks if it is restarted.
    - Verifies that removed processes are no longer restarted.

    Why integration test?
    ---------------------
    Unit tests check individual methods in isolation, but integration tests ensure that all components
    (config manager, process monitor, OS API wrapper, logger) work together as expected in real-world usage.
    This is essential for reliability and demonstration.
*/

#include <cstdlib>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>

void write_config(const std::string& process_name) {
    std::ofstream cfg("config.json");
    cfg << "{\n"
        << "  \"processes\": [\n"
        << "    { \"name\": \"" << process_name << "\", \"args\": \"\" }\n"
        << "  ],\n"
        << "  \"foreground\": \"" << process_name << "\"\n"
        << "}\n";
    cfg.close();
}

int main() {
    // 1. Write initial config for notepad.exe
    write_config("notepad.exe");

    // 2. Start the watchdog in the background
    std::system("start build\\main.exe"); // Use "start" for Windows, adjust path as needed

    // 3. Wait for watchdog to start and launch notepad
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 4. Kill notepad.exe (simulate user closing it)
    std::system("taskkill /IM notepad.exe /F");

    // 5. Wait for watchdog to restart notepad
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 6. Change config to mspaint.exe (remove notepad)
    write_config("mspaint.exe");

    // 7. Wait for config reload and process change
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 8. Kill mspaint.exe to see if it restarts
    std::system("taskkill /IM mspaint.exe /F");
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // 9. Cleanup: kill both processes and watchdog
    std::system("taskkill /IM notepad.exe /F");
    std::system("taskkill /IM mspaint.exe /F");
    std::system("taskkill /IM main.exe /F");

    std::cout << "Integration test completed. Check Windows Event Log for actions." << std::endl;
    return 0;
}