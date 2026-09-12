#include "Console.h"
#include "Debug.h"
#include "StringUtility.h"
#include "Syntax.h"
#include <cstddef>

#ifdef _WIN32
#include <Windows.h>
#else
#include <cstdio>
#include <iostream>
#endif

Console::Console() : allocated(false) {

}

void Console::Allocate() {
#ifdef _WIN32
    if (!allocated) {
        AllocConsole();
        allocated = true;
    }
#endif
}

void Console::WriteLine(wstring_view message) {

    Allocate();

    wstring messageString(message);

    // Output within the Visual Studio output window.
    Debug::Log(messageString);

#ifdef _WIN32
    // Output within the command line window.
    DWORD dwBuff = 0;
    auto hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(hStdOut, messageString.c_str(), messageString.length(), &dwBuff, NULL);  // #.c_str() OK
#else
    std::fwprintf(stdout, L"%ls", messageString.c_str());
#endif

}

void Console::Write(wstring_view message) {
    WriteLine(message);

}

void  Console::Write(wstring_view pattern, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3) {
    WriteLine(String::Format(pattern, v0, v1, v2, v3));
}

string Console::ReadLine() {

#ifdef _WIN32
    constexpr int BUFFER_LENGTH = 1024;
    constexpr auto STD_HANDLE = STD_INPUT_HANDLE;
    CHAR szBuffer[BUFFER_LENGTH] = {};

    DWORD dwRead = 0;
    auto hIn = GetStdHandle(STD_HANDLE);

    if (hIn == INVALID_HANDLE_VALUE) {
        safeExit("Console::ReadLine(): Invalid handle value.");
    }

    if (ReadConsole(hIn, szBuffer, sizeof(szBuffer), &dwRead, NULL)) {
        // Loop until RETURN pressed
    }
    return string(szBuffer);
#else
    string line;
    std::getline(std::cin, line);
    return line;
#endif
}