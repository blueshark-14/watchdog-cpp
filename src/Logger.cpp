#include "Logger.h"

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