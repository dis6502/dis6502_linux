#pragma once

#include "Syntax.h"
#include "resource.h"

/**
* Format texts from application's resource file.
**/
class Text {
public:

    static constexpr size_t MAX_STRING_LENGTH = 250;

    using TextID = unsigned int;

    static wstring Get(TextID textID);
    static wstring Format(TextID textID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");
};