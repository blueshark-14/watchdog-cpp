#pragma once
#include "ConfigManager.h"
#include "OSApiWrapper.h"
#include <unordered_map>
#include <atomic>
#include <functional>

class ProcessMonitor {
public:
    ProcessMonitor(ConfigManager& cfg, OSApiWrapper& api);
    // Add a run method that takes a stop condition
    void run(std::function<bool()> keepRunning);
    void stop();        
private:
    ConfigManager& cfg;
    OSApiWrapper& api;
    std::unordered_map<std::string, ProcessInfo> monitored; // name -> info
    std::atomic<bool> running{true};
};