#pragma once

#include "Syntax.h"

// File objects are immutable values representing a file or folder
class File {
public:

    File() = default;
    File(wstring_view filePath);
    File(const File& parentFile, wstring_view fileName);

    File(const File&);                        // copy constructor		
    File& operator=(const File&);             // copy assignment
    File(File&&) noexcept;                    // move constructor
    File& operator=(File&&) noexcept;         // move assignment

    wstring GetPath() const;
    wstring GetParentPath() const;
    wstring GetName() const;
    wstring GetExtension() const;

    bool Exists() const;

private:
    wstring filePath;

};