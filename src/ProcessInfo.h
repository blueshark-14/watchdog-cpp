#pragma once
#include <string>

class ProcessInfo {
public:
    ProcessInfo() : name(""), args("") {} 
    ProcessInfo(const std::string& name, const std::string& args)
        : name(name), args(args) {}
    std::string getName() const { return name; }
    std::string getArgs() const { return args; }
private:
    std::string name;
    std::string args;
};