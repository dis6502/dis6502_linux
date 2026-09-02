#pragma once
#include "Syntax.h"

class Writer
{
public:
    virtual void Write(wstring_view pattern, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"") = 0;
};