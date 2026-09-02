#include "OperatingSystem.h"
#ifndef _WIN32
#include "StringUtility.h"
#endif
#include "Syntax.h"
#include <cstdlib>


int OperatingSystem::ExecuteCommand(wstring_view command) {
    auto commandString = wstring(command);
#ifdef _WIN32
    const auto result = _wsystem(commandString.c_str());
#else
    const auto result = std::system(String::wstring_to_utf8(commandString).c_str());
#endif
    return result;
}

