#pragma once

#include "Syntax.h"

// Small piece of test infrastructure extracted out of MainTest (which is
// otherwise UI/controller-layer code - see ui_todo/MainTest.*): resolves
// the on-disk location of tst/suite/, used by the various *Test.cpp unit
// tests to find their fixture input/reference/output files.
namespace TestPaths {
    wstring GetSuitePath();
}
