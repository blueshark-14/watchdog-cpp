/*
    NOTE ON PROCESSENTRY32 AND UNICODE SUPPORT

    When using the Windows ToolHelp API (PROCESSENTRY32, Process32First, etc.),
    be aware that the structure and functions can use either ANSI (char) or Unicode (WCHAR) strings,
    depending on whether UNICODE is defined in your build environment.

    If your project is built with UNICODE enabled (the default for many modern Windows toolchains),
    PROCESSENTRY32::szExeFile is WCHAR[260], not char[260].

    Attempting to construct a std::string directly from a WCHAR array will cause a build error:
        "no instance of constructor std::string matches the argument list (WCHAR[260])"

    To compare process names, you must convert WCHAR* to std::string.
    This file uses a helper function ws2s(const WCHAR*) to perform this conversion using WideCharToMultiByte.

    If you use the ANSI version (PROCESSENTRY32A), you can compare directly with std::string,
    but for Unicode compatibility, prefer the WCHAR version and explicit conversion.

    Example usage:
        if (name == ws2s(pe.szExeFile)) { ... }

    This approach ensures your code works regardless of the UNICODE setting and avoids subtle bugs.
*/
/*
    OSApiWrapper.cpp - Windows Process Management and Unicode Handling

    This file implements process management functions for Windows, such as checking if a process is running,
    starting, killing, and bringing a process to the foreground.

    IMPORTANT NOTE ON UNICODE AND PROCESSENTRY32:

    - The Windows ToolHelp API (PROCESSENTRY32, Process32First, etc.) provides both ANSI (A) and Unicode (W) versions.
    - If your project is built with UNICODE enabled (the default for many modern Windows toolchains),
      PROCESSENTRY32::szExeFile is WCHAR[260], not char[260].
    - Attempting to construct a std::string directly from a WCHAR array will cause a build error:
        "no instance of constructor std::string matches the argument list (WCHAR[260])"
    - To compare process names, you must convert WCHAR* to std::string.
      This file uses a helper function ws2s(const WCHAR*) to perform this conversion using WideCharToMultiByte.
    - If you use the ANSI version (PROCESSENTRY32A), you can compare directly with std::string,
      but for Unicode compatibility, prefer the WCHAR version and explicit conversion.

    Example usage:
        if (name == ws2s(pe.szExeFile)) { ... }

    This approach ensures your code works regardless of the UNICODE setting and avoids subtle bugs.

    See also:
    - https://learn.microsoft.com/en-us/windows/win32/api/tlhelp32/ns-tlhelp32-processentry32
    - https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
*/
/*
    Why do we need a static callback function for EnumWindows?

    The Windows API function EnumWindows expects a function pointer of type:
        BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

    This must be a regular function (or a static function), not a C++ lambda with captures or a member function,
    because only a plain function pointer can be passed to the C API.

    Lambdas without captures can sometimes be converted to function pointers, but this is not portable or reliable,
    and lambdas with captures (which reference local variables or class members) cannot be used at all.

    By defining a static callback function (EnumWindowsProc), we ensure compatibility with the EnumWindows API,
    and can safely pass user data (such as our EnumData struct) via the LPARAM argument.

    This is a common pattern when working with Windows API enumeration functions.
*/


#include "OSApiWrapper.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "Logger.h"

// Converts a wide string (WCHAR*) to a UTF-8 std::string
// This is needed because PROCESSENTRY32W::szExeFile is WCHAR[260] (wide string),
// but we want to compare it with std::string (UTF-8).
static std::string ws2s(const WCHAR* wstr) {
    if (!wstr) return {}; // Return empty string if input is null
    // Get the required buffer size for the UTF-8 string
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 1) return {}; // Return empty if conversion fails or string is empty
    std::string strTo(size_needed - 1, 0); // Allocate string of required size (excluding null terminator)
    // Perform the conversion from wide string to UTF-8
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

// Helper for case-insensitive comparison
static bool iequals(const std::string& a, const std::string& b) {
    return a.size() == b.size() &&
        std::equal(
            a.begin(), a.end(),
            b.begin(),
            [](char a, char b) {
                return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
            }
        );
}

// Checks if a process with the given name is running
bool OSApiWrapper::isProcessRunning(const std::string& name) {
    std::cout << "Checking if process is running: " << name << std::endl;
    // Take a snapshot of all processes in the system
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
     if (hSnap == INVALID_HANDLE_VALUE) {
        logToWindowsEventLog("Failed to take process snapshot for: " + name, EVENTLOG_ERROR_TYPE);
        return false;
    }
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    bool found = false;

    // Iterate through all processes in the snapshot
    if (Process32FirstW(hSnap, &pe)) {
        do {
            // Convert process name from WCHAR* to std::string and compare
            if (iequals(name, ws2s(pe.szExeFile))) {
                found = true; // Found a matching process
                break;
            }
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap); // Always close the snapshot handle
    if (found) {
        logToWindowsEventLog("Process is running: " + name);
    } else {
        logToWindowsEventLog("Process is NOT running: " + name, EVENTLOG_WARNING_TYPE);
    }
    return found;
}

void OSApiWrapper::startProcess(const std::string& exe, const std::string& args) {
    // Convert the executable and arguments from UTF-8 std::string to wide string (std::wstring)
    std::wstring wexe(exe.begin(), exe.end());
    std::wstring wargs(args.begin(), args.end());
    // Combine executable and arguments into a single command line string
    std::wstring cmd = wexe + L" " + wargs;

    // Initialize STARTUPINFOW structure for process creation
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Create the process using the wide-character Windows API
    if (CreateProcessW(
            nullptr,        // Application name (nullptr means use command line)
            &cmd[0],        // Command line (must be modifiable, hence &cmd[0])
            nullptr,        // Process security attributes
            nullptr,        // Thread security attributes
            FALSE,          // Inherit handles
            0,              // Creation flags
            nullptr,        // Environment
            nullptr,        // Current directory
            &si,            // Startup info
            &pi)) {         // Process information (receives handles)
        // Close process and thread handles to avoid resource leaks
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        std::cout << "Started process: " << exe << " " << args << std::endl;
        logToWindowsEventLog("Started process: " + exe + " " + args);
    } else {
        logToWindowsEventLog("Failed to start process: " + exe + " " + args, EVENTLOG_ERROR_TYPE);
    }
}

void OSApiWrapper::killProcess(const std::string& name) {
    // Take a snapshot of all processes in the system
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) {
        logToWindowsEventLog("Failed to take process snapshot for kill: " + name, EVENTLOG_ERROR_TYPE);
        return;
    }
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);
    bool killed = false;

    // Iterate through all processes in the snapshot
    if (Process32FirstW(hSnap, &pe)) {
        do {
            // Compare process name (converted from WCHAR* to std::string)
            if (name == ws2s(pe.szExeFile)) {
                // Open the process with permission to terminate
                HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                if (hProc) {
                    // Terminate the process
                    TerminateProcess(hProc, 0);
                    CloseHandle(hProc); // Close the process handle
                    std::cout << "Killed process: " << name << std::endl;
                    logToWindowsEventLog("Killed process: " + name, EVENTLOG_WARNING_TYPE);
                    killed = true;
                }
            }
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap); // Always close the snapshot handle
    if (!killed) {
        logToWindowsEventLog("No running process found to kill: " + name, EVENTLOG_WARNING_TYPE);
    }
}

// Static callback function for EnumWindows
static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam) {
    struct EnumData {
        DWORD pid;
        HWND hwnd;
    };
    EnumData* pData = reinterpret_cast<EnumData*>(lParam);
    DWORD winPid = 0;
    GetWindowThreadProcessId(hWnd, &winPid);
    if (winPid == pData->pid && IsWindowVisible(hWnd)) {
        pData->hwnd = hWnd;
        return FALSE; // Stop enumeration
    }
    return TRUE; // Continue enumeration
}

void OSApiWrapper::bringToForeground(const std::string& name) {
    // Find the process ID (pid) for the given process name
    DWORD pid = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(PROCESSENTRY32W);

    // Iterate through all processes to find the matching name
    if (Process32FirstW(hSnap, &pe)) {
        do {
            // Convert process name from WCHAR* to std::string and compare
            if (name == ws2s(pe.szExeFile)) {
                pid = pe.th32ProcessID; // Store the process ID
                break;
            }
        } while (Process32NextW(hSnap, &pe));
    }
    CloseHandle(hSnap); // Always close the snapshot handle

    if (pid == 0) {
        // No process found with the given name
        std::cerr << "Process not found: " << name << std::endl;
        logToWindowsEventLog("Process not found for foreground: " + name, EVENTLOG_WARNING_TYPE);
        return;
    }

    // Prepare to search for the main window belonging to the process
    HWND hwnd = nullptr;
    struct EnumData { DWORD pid; HWND hwnd; };
    EnumData data = { pid, nullptr };

    // Use the static callback function instead of a lambda
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
    if (data.hwnd) {
        SetForegroundWindow(data.hwnd);
        std::cout << "Brought process to foreground: " << name << std::endl;
        logToWindowsEventLog("Brought process to foreground: " + name);
    } else {
        std::cerr << "No window found for process: " << name << std::endl;
        logToWindowsEventLog("No window found for process: " + name, EVENTLOG_WARNING_TYPE);
    }
}