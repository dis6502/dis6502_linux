#pragma once

#include "Syntax.h"

class FileIOTest {
public:
    static void AssertFileEquals(wstring_view actualFilePath, wstring_view expectedFilePath);

    static void TestFileIO();
};
