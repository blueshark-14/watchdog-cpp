/*
    Unit Tests for ConfigManager

    Why unit test?
    --------------
    Unit tests ensure that each method in ConfigManager works as expected in isolation,
    regardless of the rest of the system. This increases reliability and makes future
    changes safer and easier to validate.

    These tests cover:
    - Loading a valid config file
    - Handling missing or invalid config files
    - Dynamic reload detection
    - Correct parsing of processes and foreground app
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ConfigManager.h"
#include <fstream>   // For file I/O
#include <cstdio>    // For std::remove
#include <thread>    // For sleep
#include <chrono>    // For sleep duration

// Helper: Write a config file for testing
void write_test_config(const std::string& path, const std::string& proc, const std::string& args, const std::string& fg) {
    std::ofstream f(path); // Open file for writing
    f << "{\n"
      << "  \"processes\": [\n"
      << "    { \"name\": \"" << proc << "\", \"args\": \"" << args << "\" }\n"
      << "  ],\n"
      << "  \"foreground\": \"" << fg << "\"\n"
      << "}\n";
    f.close(); // Close file
}

TEST_CASE("ConfigManager loads valid config", "[config]") {
    std::string path = "test_config.json";
    write_test_config(path, "notepad.exe", "", "notepad.exe"); // Create a test config file

    ConfigManager cfg(path); // Load config
    auto procs = cfg.getProcesses(); // Get processes from config
    REQUIRE(procs.size() == 1); // Should have one process
    REQUIRE(procs[0].getName() == "notepad.exe"); // Name should match
    REQUIRE(procs[0].getArgs() == ""); // Args should match
    REQUIRE(cfg.getForegroundApp() == "notepad.exe"); // Foreground app should match

    std::remove(path.c_str()); // Clean up: delete the test config file after test
    // This prevents leftover files from affecting other tests or cluttering the directory
}

TEST_CASE("ConfigManager handles missing config file", "[config]") {
    std::string path = "missing_config.json";
    ConfigManager cfg(path); // Try to load a non-existent config file
    REQUIRE(cfg.getProcesses().empty()); // Should return an empty process list
    // No file to remove here, as it was never created
}

TEST_CASE("ConfigManager detects config reload", "[config]") {
    std::string path = "test_reload.json";
    write_test_config(path, "notepad.exe", "", "notepad.exe"); // Write initial config
    ConfigManager cfg(path); // Load config

    // Wait to ensure file modification time changes (important for reload detection)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    write_test_config(path, "mspaint.exe", "", "mspaint.exe"); // Overwrite config with new process

    REQUIRE(cfg.reloadIfChanged() == true); // Should detect the change and reload
    auto procs = cfg.getProcesses(); // Get updated processes
    REQUIRE(procs.size() == 1); // Should have one process
    REQUIRE(procs[0].getName() == "mspaint.exe"); // Name should match new config
    REQUIRE(cfg.getForegroundApp() == "mspaint.exe"); // Foreground app should match new config

    std::remove(path.c_str()); // Clean up: delete the test config file after test
}