#pragma once

#include "Syntax.h"
class DisassemblyResult;

class DisassemblyResultTest {
public:

    static void TestDisassemblyResult();

    static void GenerateDisassemblyResult(DisassemblyResult& result, size_t linesPerSection);

};