#pragma once
#include <string>
#include <vector>
#include "ProcessInfo.h"

class ConfigManager {
public:
    ConfigManager(const std::string& path);
    void load();
    void watchForChanges(); // reload on file changes
    std::vector<ProcessInfo> getProcesses() const;
    std::string getForegroundApp() const;
private:
    std::string filepath;
};