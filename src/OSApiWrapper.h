#pragma once
#include <string>

class OSApiWrapper {
public:
    OSApiWrapper() = default;
    ~OSApiWrapper() = default;

    bool isProcessRunning(const std::string& name);
    void startProcess(const std::string& exe, const std::string& args);
    void killProcess(const std::string& name);
    void bringToForeground(const std::string& name);
};