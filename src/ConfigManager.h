#pragma once
#include <string>
#include <vector>
#include <ctime>        
#include "ProcessInfo.h"

class ConfigManager {
public:
    ConfigManager(const std::string& path);
    void load();
    void watchForChanges(); // reload on file changes
    bool reloadIfChanged();
    std::vector<ProcessInfo> getProcesses() const;
    std::string getForegroundApp() const;
private:
    std::string filepath;
    std::vector<ProcessInfo> processes;
    std::string foregroundApp;
    std::time_t lastModified = 0; // track last modified time
};