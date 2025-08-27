#pragma once
#include "ConfigManager.h"
#include "OSApiWrapper.h"

class ProcessMonitor {
public:
    ProcessMonitor(ConfigManager& cfg, OSApiWrapper& api);
    void run();
private:
    ConfigManager& cfg;
    OSApiWrapper& api;
};