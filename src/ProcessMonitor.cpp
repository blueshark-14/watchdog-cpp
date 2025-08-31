#include "ProcessMonitor.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "Logger.h" 

ProcessMonitor::ProcessMonitor(ConfigManager& cfg, OSApiWrapper& api)
    : cfg(cfg), api(api) {}

void ProcessMonitor::run(std::function<bool()> keepRunning) {
    std::unordered_map<std::string, ProcessInfo> monitored;
    for (const auto& p : cfg.getProcesses()) {
        monitored[p.getName()] = p;
    }

    while (keepRunning()) {
        // Reload config if changed
        if (cfg.reloadIfChanged()) {
            std::unordered_map<std::string, ProcessInfo> newMonitored;
            for (const auto& p : cfg.getProcesses()) {
                newMonitored[p.getName()] = p;
                if (monitored.find(p.getName()) == monitored.end()) {
                    if (!api.isProcessRunning(p.getName()))
                        api.startProcess(p.getName(), p.getArgs());
                }
            }
            for (auto it = monitored.begin(); it != monitored.end(); ++it) {
                const std::string& name = it->first;
                if (newMonitored.find(name) == newMonitored.end()) {
                    logToWindowsEventLog("Stopped monitoring: " + name, EVENTLOG_WARNING_TYPE);
                }
            }
            monitored = std::move(newMonitored);
            // Bring the new foreground app to the foreground after config reload
            api.bringToForeground(cfg.getForegroundApp());
        }

        // Ensure all monitored processes are running
        for (auto it = monitored.begin(); it != monitored.end(); ++it) {
            const std::string& name = it->first;
            const ProcessInfo& info = it->second;
            if (!api.isProcessRunning(name)) {
                logToWindowsEventLog("Process stopped, restarting: " + name, EVENTLOG_WARNING_TYPE);
                api.startProcess(name, info.getArgs());
            }
        }
         // Always enforce the configured foreground app is in the foreground
        const std::string& fgApp = cfg.getForegroundApp();
        if (!fgApp.empty() && !api.isProcessInForeground(fgApp)) {
            api.bringToForeground(fgApp);
        }   

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}