#include "ProcessMonitor.h"
#include "ProcessInfo.h"
#include "ConfigManager.h"
#include <iostream>

ProcessMonitor::ProcessMonitor(ConfigManager& cfg, OSApiWrapper& api)
    : cfg(cfg), api(api) {
    std::cout << "ProcessMonitor created." << std::endl;
}

void ProcessMonitor::run() {
    std::cout << "ProcessMonitor running..." << std::endl;

    std::vector<ProcessInfo> processes = cfg.getProcesses();
    for (const auto& proc : processes) {
        std::cout << "Would monitor process: " << proc.getName() << " with args: " << proc.getArgs() << std::endl;
    }
    std::cout << "Foreground app should be: " << cfg.getForegroundApp() << std::endl;
}