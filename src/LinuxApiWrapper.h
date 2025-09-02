#pragma once
#include "OSApiWrapper.h"
#include <vector>
#include <string>

// Concrete implementation of OSApiWrapper for Linux
class LinuxApiWrapper : public OSApiWrapper {
public:
    LinuxApiWrapper() = default;
    ~LinuxApiWrapper() override = default;

    bool isProcessRunning(const std::string& name) override;
    void startProcess(const std::string& exe, const std::string& args) override;
    void killProcess(const std::string& name) override;
    void bringToForeground(const std::string& name) override;
    bool isProcessInForeground(const std::string& name) override;
};