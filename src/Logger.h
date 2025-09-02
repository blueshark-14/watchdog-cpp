#pragma once
#include <string>

#ifdef _WIN32
#include <windows.h>
void logToWindowsEventLog(const std::string& message, WORD type = EVENTLOG_INFORMATION_TYPE);
#define WDOG_LOG_WARNING EVENTLOG_WARNING_TYPE
#else
void logToWindowsEventLog(const std::string& message, int type = 0);
#define WDOG_LOG_WARNING 0
#endif