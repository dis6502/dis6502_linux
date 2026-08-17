#pragma once
#include "Syntax.h"

// Portable stand-in for the Win32 MessageBoxDialog. A real UI layer
// (ImGui, etc.) should replace Show()'s implementation with an actual
// modal popup; parentWindow is kept as an opaque pointer so call sites
// (which currently only ever pass nullptr) don't need to change.
constexpr unsigned int MB_OK = 0;

class MessageBoxDialog {
public:
    static int Show(void* parentWindow, wstring_view title, wstring_view message, unsigned int uType = MB_OK);
};
