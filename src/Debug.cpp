#include "Debug.h"
#include "Syntax.h"
#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif

static unsigned long count;

void Debug::Log(wstring_view text) {

#ifdef _WIN32
    OutputDebugString((std::to_wstring(count) + L": " + wstring(text) + L"\n").c_str());
#else
    std::fwprintf(stderr, L"%ls", text);
#endif
    count++;
}

void Debug::LogValue(wstring_view label, wstring_view value) {
    Log(wstring(label) + L": " + wstring(value));
}
