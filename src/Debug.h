#pragma once

#include "Syntax.h"

class Debug {
public:

    static void Log(wstring_view text);
    static void LogValue(wstring_view label, wstring_view value);

};