#include "Debug.h"

#include "PlatformCompat.h"

static unsigned long count;

void Debug::Log(wstring_view text) {
    OutputDebugString((std::to_wstring(count) + L": " + wstring(text) + L"\n").c_str());
    count++;
}

void Debug::LogValue(wstring_view label, wstring_view value) {
    Log(wstring(label) + L": " + wstring(value));
}
