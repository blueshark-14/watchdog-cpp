#pragma once
#include "OSApiWrapper.h"

// Concrete implementation of OSApiWrapper for Windows
class WindowsApiWrapper : public OSApiWrapper {
public:
    WindowsApiWrapper() = default;
    ~WindowsApiWrapper() override = default;

    bool isProcessRunning(const std::string& name) override;
    void startProcess(const std::string& exe, const std::string& args) override;
    void killProcess(const std::string& name) override;
    void bringToForeground(const std::string& name) override;
};