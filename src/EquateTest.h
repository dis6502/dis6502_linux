#pragma once

#include "Syntax.h"

class EquateTest {
public:
    static void TestEquate();
    static void TestFiles(wstring_view filePath);


private:
    static void TestParsing();

};