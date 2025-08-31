#pragma once
#include <string>

class OSApiWrapper {
public:
    // Make the destructor virtual to ensure that when deleting an object through a base class pointer,
    // the derived class's destructor is called. This prevents resource leaks and undefined behavior
    // when using polymorphism (e.g., OSApiWrapper* api = new DerivedApi(); delete api;).
    virtual ~OSApiWrapper() = default;

    virtual bool isProcessRunning(const std::string& name) = 0;
    virtual void startProcess(const std::string& exe, const std::string& args) = 0;
    virtual void killProcess(const std::string& name) = 0;
    virtual void bringToForeground(const std::string& name) = 0;
    virtual bool isProcessInForeground(const std::string& name) = 0;
};