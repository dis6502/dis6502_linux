#include "Console.h"
#include "Debug.h"
#include "Strings.h"
#include <Windows.h>

Console::Console() {
    allocated = false;
}

void Console::Allocate() {
    if (!allocated) {
        AllocConsole();
        allocated = true;
    }
}

void Console::WriteLine(wstring_view message) {

    Allocate();

    wstring messageString(message);

    // Output within Visual Studio
    Debug::Log(messageString);

    // Output within command line window
    DWORD dwBuff = 0;
    auto hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsole(hStdOut, messageString.c_str(), messageString.length(), &dwBuff, NULL);  // #.c_str() OK

}

void Console::Write(wstring_view message) {
    WriteLine(message);

}

void  Console::Write(wstring_view pattern, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3) {
    WriteLine(String::Format(pattern, v0, v1, v2, v3));
}

string Console::ReadLine() {

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
}