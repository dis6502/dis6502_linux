#pragma once

#include "Syntax.h"
#include "FileIO.h"

class AtariDiskImageTest {
public:
    static void AssertFileNames83Equal();
    static void AssertDirectoryEquals(wstring_view diskImageFilePath, wstring_view expectedDirectory);
    
private:
    void static AssertFileName83Equals(wstring_view fileName, wstring_view expectedFileName83);
    void static AssertFileContentEquals(wstring_view diskImageFilePath, wstring_view fileName, wstring_view expectedFilePath);

    static wstring GetDirectory(wstring_view diskImageFilePath);
    static ByteArray GetFileContent(wstring_view diskImageFilePath, wstring_view fileName, FileIO::FILE_SIZE& readCount);
};