/*
    Unit Tests for ProcessMonitor

    How these unit tests work:
    --------------------------
    - These tests use a mock OSApiWrapper and a mock ConfigManager to simulate process monitoring behavior
      without actually starting or killing real processes.
    - By injecting mocks, we can verify that ProcessMonitor calls the correct OSApiWrapper methods
      in response to config changes and process state changes.
    - This approach ensures ProcessMonitor's logic is correct in isolation, independent of the OS.

    What is tested:
    - Starts missing processes on startup
    - Restarts processes if they are stopped
    - Starts new processes when added to config
    - Stops monitoring (and does not restart) processes removed from config
*/
/*
  OOP Principles Applied
  -----------------------
  - Encapsulation: ProcessMonitor encapsulates the logic for monitoring processes, providing a clean interface for interaction.
  - Abstraction: The use of OSApiWrapper and ConfigManager abstracts away platform-specific details and configuration management.
  - Polymorphism: MockApi and MockConfig demonstrate polymorphism, allowing for flexible testing without changing the core logic.
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ProcessMonitor.h"
#include "ConfigManager.h"
#include "ProcessInfo.h"
#include <vector>
#include <string>
#include <functional>


// Dummy logger for unit tests:
// This prevents linker errors when ConfigManager or other code calls logToWindowsEventLog.
// In unit tests, we don't need to actually log to the Windows Event Log.
void logToWindowsEventLog(const std::string&, unsigned short) {}

// --- Mock Classes ---

class MockApi : public OSApiWrapper {
public:
    std::vector<std::string> started;
    std::vector<std::string> killed;
    std::vector<std::string> checked;
    std::vector<std::string> running; // Simulate running processes

    bool isProcessRunning(const std::string& name) override {
        checked.push_back(name);
        return std::find(running.begin(), running.end(), name) != running.end();
    }
    void startProcess(const std::string& name, const std::string& args) override {
        started.push_back(name);
        running.push_back(name);
    }
    void killProcess(const std::string& name) override {
        killed.push_back(name);
        running.erase(std::remove(running.begin(), running.end(), name), running.end());
    }
    void bringToForeground(const std::string&) override {}
    bool isProcessInForeground(const std::string&) override { return true; }    
};

class MockConfig : public ConfigManager {
    std::vector<ProcessInfo> procs;
    std::string fg;
    bool changed = false;
public:
    MockConfig(const std::vector<ProcessInfo>& p, const std::string& f) : ConfigManager(""), procs(p), fg(f) {}
    const std::vector<ProcessInfo>& getProcesses() const override { return procs; }
    const std::string& getForegroundApp() const override { return fg; }
    bool reloadIfChanged() override {
        if (changed) { changed = false; return true; }
        return false;
    }
    void setProcesses(const std::vector<ProcessInfo>& p) { procs = p; changed = true; }
};

// --- Tests ---

TEST_CASE("ProcessMonitor starts missing processes on startup", "[ProcessMonitor]") {
    MockApi api;
    std::vector<ProcessInfo> procs = { ProcessInfo("notepad.exe", ""), ProcessInfo("mspaint.exe", "") };
    MockConfig cfg(procs, "notepad.exe");
    ProcessMonitor monitor(cfg, api);

    // Simulate one run loop
    bool ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // Both processes should be started (since none are running)
    REQUIRE(api.started.size() == 2);
    REQUIRE(std::find(api.started.begin(), api.started.end(), "notepad.exe") != api.started.end());
    REQUIRE(std::find(api.started.begin(), api.started.end(), "mspaint.exe") != api.started.end());
}

TEST_CASE("ProcessMonitor restarts stopped processes", "[ProcessMonitor]") {
    MockApi api;
    api.running = { "notepad.exe" }; // Only notepad is running
    std::vector<ProcessInfo> procs = { ProcessInfo("notepad.exe", ""), ProcessInfo("mspaint.exe", "") };
    MockConfig cfg(procs, "notepad.exe");
    ProcessMonitor monitor(cfg, api);

    bool ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // Only mspaint.exe should be started
    REQUIRE(api.started.size() == 1);
    REQUIRE(api.started[0] == "mspaint.exe");
}

TEST_CASE("ProcessMonitor starts new process when added to config", "[ProcessMonitor]") {
    MockApi api;
    api.running = { "notepad.exe" };
    std::vector<ProcessInfo> procs = { ProcessInfo("notepad.exe", "") };
    MockConfig cfg(procs, "notepad.exe");
    ProcessMonitor monitor(cfg, api);

    // Simulate initial run
    bool ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // Add mspaint.exe to config and trigger reload
    cfg.setProcesses({ ProcessInfo("notepad.exe", ""), ProcessInfo("mspaint.exe", "") });
    ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // mspaint.exe should be started
    REQUIRE(std::find(api.started.begin(), api.started.end(), "mspaint.exe") != api.started.end());
}

TEST_CASE("ProcessMonitor stops monitoring removed processes", "[ProcessMonitor]") {
    MockApi api;
    api.running = { "notepad.exe", "mspaint.exe" };
    std::vector<ProcessInfo> procs = { ProcessInfo("notepad.exe", ""), ProcessInfo("mspaint.exe", "") };
    MockConfig cfg(procs, "notepad.exe");
    ProcessMonitor monitor(cfg, api);

    // Initial run
    bool ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // Remove mspaint.exe from config and trigger reload
    cfg.setProcesses({ ProcessInfo("notepad.exe", "") });
    ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // mspaint.exe should not be restarted if killed
    api.running.erase(std::remove(api.running.begin(), api.running.end(), "mspaint.exe"), api.running.end());
    ran = false;
    monitor.run([&ran]() { if (ran) return false; ran = true; return true; });

    // Only notepad.exe should be checked/restarted
    REQUIRE(std::find(api.started.begin(), api.started.end(), "mspaint.exe") == api.started.end());
}