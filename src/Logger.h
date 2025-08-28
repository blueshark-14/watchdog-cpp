#pragma once
#include <string>
#include <windows.h>

// Logs a message to the Windows Event Log
void logToWindowsEventLog(const std::string& message, WORD type = EVENTLOG_INFORMATION_TYPE);