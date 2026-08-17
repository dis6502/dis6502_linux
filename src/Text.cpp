#include "Text.h"
#include "String.h"

// Portable placeholder for Win32 LoadString()-based resource lookup.
// The .rc file's actual English strings live in dis6502.rc and would need
// to be extracted into a real string table (or ideally .po/gettext files)
// for a proper UI port. For now this keeps engine code and its tests
// compiling/running without needing to link the Win32 resource system;
// it returns a clearly-marked placeholder rather than silently wrong text.
wstring Text::Get(Text::TextID textID) {
    return L"[string#" + std::to_wstring(textID) + L"]";
}

wstring Text::Format(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    auto text = Get(textID);
    return String::Format(text, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
