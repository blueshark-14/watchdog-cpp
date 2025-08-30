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
    virtual bool reloadIfChanged();
    virtual const std::vector<ProcessInfo>& getProcesses() const;
    virtual const std::string& getForegroundApp() const;

    virtual ~ConfigManager() = default;
private:
    std::string filepath;
    std::vector<ProcessInfo> processes;
    std::string foregroundApp;
    std::time_t lastModified = 0; // track last modified time
};