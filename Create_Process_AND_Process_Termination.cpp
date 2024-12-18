#define UNICODE  
#define _UNICODE 
#include <Windows.h> 
#include <iostream>
#include <Psapi.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>
using namespace std;
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <string>

STARTUPINFO si;
PROCESS_INFORMATION pi;
HANDLE new_process_handle;
////////////////////////////////////////////Create Process///////////////////////////////////

HANDLE CreateProcessTest(TCHAR* moduleName)
{

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the child process. 
    if (!CreateProcess(NULL,   // No module name (use command line)
        moduleName,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 0;
    }

    printf("Process is created successfully with PID:%d\n", pi.dwProcessId);

    return pi.hProcess;
}


void ExitProcessMethod1(HANDLE process_handle)
{
    CloseHandle(process_handle);
    printf("Process \"DID NOT\" terminated with method 1.\n");
}

void ExitProcessMethod2(HANDLE process_handle)
{
    if (TerminateProcess(process_handle, 1))
    {
        CloseHandle(process_handle);
        printf("Process terminated successfully with method 2.\n");
    }
    else
    {
        DWORD error = GetLastError();
        wchar_t buf[1024];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
        _tprintf(TEXT("Method 2 failed with error: %s (Error Code: %d)\n"), buf, error);
    }

    if (!CloseHandle(process_handle))
    {
        printf("Failed to close process handle (%d).\n", GetLastError());
    }
}


void ExitProcessMethod3(DWORD pid)
{
    char command_string[1024] = "taskkill /F /T /PID ";
    strcat_s(command_string, sizeof(command_string), to_string(pid).c_str());
    printf("command is: %s\n", command_string);

    if (system(command_string))
    {
        printf("Process terminated with method 3.\n");
    }
    else
    {
        DWORD error = GetLastError();
        wchar_t buf[1024];
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
        _tprintf(TEXT("Method 3 failed with error: %s\n"), buf);
    }
}

int main()
{    
    char path[] = "C:\\SOFTWARE\\psiphon3.exe";
    size_t new_size = strlen(path) + 1;
    wchar_t* wcstring = new wchar_t[new_size];
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, wcstring, new_size, path, _TRUNCATE);

    new_process_handle = CreateProcessTest(wcstring);
    
    //////////////////////Process Termination OR Exit the created process//////////////////////

    DWORD pid;
    printf("Wich method do you prefer for terminationg the process?");
    char method = getchar();
    switch (method)
    {
    case '1':
        ExitProcessMethod1(new_process_handle);
        break;
    case '2':
        ExitProcessMethod2(new_process_handle);
        break;
    case '3':
        pid = GetProcessId(new_process_handle);
        ExitProcessMethod3(pid);
        break;
    default:
        ExitProcessMethod1(new_process_handle);
        break;
    }
    printf("Goodbye!");
    _getch();

    return 0;
}
