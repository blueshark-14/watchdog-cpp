#include "OSApiWrapper.h"
#include <iostream>

bool OSApiWrapper::isProcessRunning(const std::string& name) {
    // TODO: Implement actual logic
    std::cout << "Checking if process is running: " << name << std::endl;
    return false;
}

void OSApiWrapper::startProcess(const std::string& exe, const std::string& args) {
    // TODO: Implement actual logic
    std::cout << "Starting process: " << exe << " " << args << std::endl;
}

void OSApiWrapper::killProcess(const std::string& name) {
    // TODO: Implement actual logic
    std::cout << "Killing process: " << name << std::endl;
}

void OSApiWrapper::bringToForeground(const std::string& name) {
    // TODO: Implement actual logic
    std::cout << "Bringing process to foreground: " << name << std::endl;
}