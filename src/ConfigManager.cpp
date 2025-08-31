/*
    ConfigManager.cpp - Dynamic Configuration Management for Watchdog

    This component is responsible for reading, parsing, and dynamically reloading the watchdog's configuration
    from a JSON file (config.json). The configuration describes which processes/applications should be monitored,
    their startup parameters, and the preferred foreground application.

    Why use nlohmann/json?
    ----------------------
    - C++ does not have built-in support for JSON parsing.
    - The nlohmann/json library is a modern, single-header, open-source C++ library that makes it easy to read,
      write, and manipulate JSON data using a syntax similar to Python or JavaScript.
    - It allows us to efficiently parse the config file, access arrays and fields, and update our process list
      in a clear and maintainable way.

    Why is this needed for dynamic config?
    --------------------------------------
    - For dynamic configuration, the program must detect changes to the config file at runtime, reload it,
      and update the monitored processes accordingly—without restarting.
    - This requires reading and parsing the JSON file multiple times during execution.
    - Using nlohmann/json makes this process robust, efficient, and easy to maintain.

    Previous implementations (static config or hardcoded values) did not require a JSON library,
    but dynamic, user-editable configuration is a professional requirement for real-world monitoring tools.

    Key Features:
    -------------
    - Reads the config file at startup and on every detected change.
    - Parses the list of processes and their arguments from JSON.
    - Supports dynamic addition and removal of monitored processes by simply editing and saving config.json.
    - Integrates seamlessly with the rest of the watchdog system for real-time process management.
*/

#include "ConfigManager.h"
#include "ProcessInfo.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <ctime>
#include "Logger.h" 
#include "json.hpp" //  JSON library
using json = nlohmann::json;

static std::time_t getFileModTime(const std::string& path) {
    struct stat result;
    if (stat(path.c_str(), &result) == 0)
        return result.st_mtime;
    return 0;
}

ConfigManager::ConfigManager(const std::string& path) : filepath(path) {
    load();
}

void ConfigManager::load() {
    std::ifstream file(filepath);
    if (!file) {
        logToWindowsEventLog("Failed to open config file: " + filepath);
        return;
    }
    json j;
    file >> j;
    processes.clear();
    for (const auto& p : j["processes"]) {
        processes.emplace_back(p["name"], p["args"]);
    }
    foregroundApp = j["foreground"];
    lastModified = getFileModTime(filepath);
}

bool ConfigManager::reloadIfChanged() {
    std::time_t mod = getFileModTime(filepath);
    if (mod != lastModified) {
        logToWindowsEventLog("Config file changed, reloading...");
        load();
        return true;
    }
    return false;
}

const std::vector<ProcessInfo>& ConfigManager::getProcesses() const { return processes; }
const std::string& ConfigManager::getForegroundApp() const { return foregroundApp; }