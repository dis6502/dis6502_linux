#pragma once

#include "Syntax.h"

/**
 * Make original C functions available without warings about unsigned char.
 */
class Character {
public:
    static bool IsSpace(char c) { return isspace(int{ c }); }
    static bool IsDigit(char c) { return isdigit(int{ c }); }
    static bool IsAlpha(char c) { return isalpha(int{ c }); }
    static bool IsAlphaNumeric(char c) { return isalnum(int{ c }); }

    static bool IsSpace(wchar_t c) { return isspace(int{ c }); }
    static bool IsDigit(wchar_t c) { return isdigit(int{ c }); }
    static bool IsAlpha(wchar_t c) { return isalpha(int{ c }); }
    static bool IsAlphaNumeric(wchar_t c) { return isalnum(int{ c }); }
};