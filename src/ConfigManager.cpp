#include "ConfigManager.h"
#include <iostream>

ConfigManager::ConfigManager(const std::string& path) : filepath(path) {
    std::cout << "ConfigManager created with file: " << filepath << std::endl;
}

void ConfigManager::load() {
    std::cout << "Loading config from: " << filepath << std::endl;
}

void ConfigManager::watchForChanges() {
    std::cout << "Watching for changes in: " << filepath << std::endl;
}

std::vector<ProcessInfo> ConfigManager::getProcesses() const {
    std::cout << "Getting processes from config..." << std::endl;
    // Return some dummy data for demonstration
    return {
        ProcessInfo("notepad.exe", ""),
        ProcessInfo("calc.exe", "")
    };
}

std::string ConfigManager::getForegroundApp() const {
    std::cout << "Getting foreground app from config..." << std::endl;
    // Return dummy data
    return "notepad.exe";
}