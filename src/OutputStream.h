#pragma once

#include "Syntax.h"
#include <gsl/gsl>
#include "Encoding.h"

class OutputStream {
public:
    static gsl::not_null<OutputStream*> OpenFile(wstring_view filePath, Encoding encoding);
    virtual ~OutputStream();

    void WriteString(wstring_view stringView); // throws IOException
    void WriteString(const wchar_t* string);  // throws IOException
    void Write(const void* buffer, long size);  // throws IOException

private:
    wstring filePath;
    int fd;
    Encoding encoding;

    OutputStream(wstring_view filePath, int fd, Encoding encoding);
};
