#include "LinuxApiWrapper.h"
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <filesystem>

// Helper: Get all PIDs for a process name by scanning /proc and matching the process name in /proc/[pid]/comm
static std::vector<pid_t> getPidsByName(const std::string& name) {
    std::vector<pid_t> pids;
    // Iterate over all directories in /proc
    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        std::string pidStr = entry.path().filename();
        // Only consider directories with numeric names (PIDs)
        if (!std::all_of(pidStr.begin(), pidStr.end(), ::isdigit)) continue;
        std::ifstream cmdline(entry.path() / "comm");
        std::string procName;
        // Read the process name from /proc/[pid]/comm
        if (cmdline >> procName) {
            if (procName == name) {
                pids.push_back(std::stoi(pidStr));
            }
        }
    }
    return pids;
}

// Returns true if any process with the given name is running
bool LinuxApiWrapper::isProcessRunning(const std::string& name) {
    return !getPidsByName(name).empty();
}

// Starts a process with the given executable and arguments using fork and execlp
void LinuxApiWrapper::startProcess(const std::string& exe, const std::string& args) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: execute the program
        if (!args.empty()) {
            execlp(exe.c_str(), exe.c_str(), args.c_str(), (char*)nullptr);
        } else {
            execlp(exe.c_str(), exe.c_str(), (char*)nullptr);
        }
        // If execlp fails, print an error and exit
        std::cerr << "Failed to start process: " << exe << std::endl;
        _exit(1);
    }
    // Parent process does nothing
}

// Sends SIGTERM to all processes with the given name
void LinuxApiWrapper::killProcess(const std::string& name) {
    auto pids = getPidsByName(name);
    for (pid_t pid : pids) {
        kill(pid, SIGTERM);
    }
}

// Not implemented: Bringing a process window to the foreground is not generally possible in Linux CLI.
// Would require X11/Wayland scripting (e.g., xdotool). Here, just print a message.
void LinuxApiWrapper::bringToForeground(const std::string& name) {
    std::cout << "[LinuxApiWrapper] bringToForeground is not implemented on Linux." << std::endl;
}

// Not implemented: Checking if a process is in the foreground is not generally possible in Linux CLI.
// Would require X11/Wayland scripting. Always returns false.
bool LinuxApiWrapper::isProcessInForeground(const std::string& name) {
    return false;
}