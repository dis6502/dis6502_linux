#include "StringUtility.h"
#include "Text.h"
#include "UIApplication.h"

extern std::unique_ptr<UIApplication> g_UIApplication;

wstring Text::Get(Text::TextID textID) {

    wchar_t szText[MAX_STRING_LENGTH];

    if (::LoadString(g_UIApplication->GetInstanceHandle(), textID, szText, sizeof(szText)) == 0) {
        wsprintfW(szText, L"String with ID %u not found.", textID);
    }
    return wstring(szText);
}

wstring Text::Format(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    auto text = Get(textID);
    return String::Format(text, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}
