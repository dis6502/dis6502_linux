#pragma once

#include "Encoding.h"
#include "FileIO.h"
#include "Syntax.h"
#include <gsl/gsl>

class OutputStream {
public:
    static gsl::not_null<OutputStream*> OpenFile(wstring_view filePath, Encoding encoding);
    virtual ~OutputStream();

    void WriteString(wstring_view stringView); // throws IOException
    void WriteString(const wchar_t* string);  // throws IOException
    void Write(const void* buffer, long size);  // throws IOException

private:
    wstring filePath;
    FILE* file;
    Encoding encoding;

    OutputStream(wstring_view filePath, FILE* file, Encoding encoding);
};
