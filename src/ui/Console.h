#pragma once

#include "Syntax.h"
#include "Writer.h"

class Console : public Writer {
public:
    Console();
    void Allocate();
    void Write(wstring_view message);
    void Write(wstring_view pattern, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"") override;
    string ReadLine();

private:
    bool allocated;

    void WriteLine(wstring_view message);
};
