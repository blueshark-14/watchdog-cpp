#include "Logger.h"
#include <iostream>

#ifdef _WIN32
void logToWindowsEventLog(const std::string& message, WORD type) {
    HANDLE hEventLog = RegisterEventSourceA(NULL, "WatchdogCPP");
    if (hEventLog) {
        LPCSTR messages[1];
        messages[0] = message.c_str();
        ReportEventA(
            hEventLog,
            type,
            0,
            0,
            NULL,
            1,
            0,
            messages,
            NULL
        );
        DeregisterEventSource(hEventLog);
    }
}
#else
void logToWindowsEventLog(const std::string& message, int type) {
    std::cout << "[LOG] " << message << std::endl;
}
#endif