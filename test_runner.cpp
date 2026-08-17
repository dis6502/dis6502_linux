// Headless runner for the real dis6502 unit test suite (the tests
// themselves are the project's own, unmodified *Test.cpp classes) -
// this replaces only the UI-coupled MainTest driver (which pulled in
// FileSystemLogic/console/etc.) with a plain console harness, so we can
// verify the ported core engine against its real fixtures in tst/suite/.
#include <cstdio>
#include <exception>

#include "Application.h"
#include "TestPaths.h"
#include "Assert.h"

#include "CommonTest.h"
#include "FileIOTest.h"
#include "MemoryInspectorTest.h"
#include "EquateTest.h"
#include "DisassemblyResultTest.h"
#include "DisassemblyResultFileTest.h"
#include "SegmentTest.h"
#include "WorkspaceLogic.h"
#include "Workspace.h"
#include "systems/ComputerSystemFactory.h"
#include "systems/ComputerSystemTest.h"
#include "systems/atari800/AtariDiskImageTest.h"

int main() {
    int failed = 0;
    auto run = [&](const char* name, auto fn) {
        std::printf("--- %s ---\n", name);
        try {
            fn();
            std::printf("PASS: %s\n", name);
        } catch (const std::exception& ex) {
            failed++;
            std::printf("FAIL: %s : %s\n", name, ex.what());
        }
    };

    run("CommonTest", [] { CommonTest::TestCommon(); });
    run("FileIOTest", [] { FileIOTest::TestFileIO(); });
    run("MemoryInspectorTest", [] { MemoryInspectorTest::Test(); });

    run("AtariDiskImageTest", [] {
        AtariDiskImageTest::AssertFileNames83Equal();
        auto filePath = TestPaths::GetSuitePath() + L"/diskimage/ataridiskimage/in/DISK_NOT_FOUND.atr";
        AtariDiskImageTest::AssertDirectoryEquals(filePath, L"DISK_NOT_FOUND\n");
    });

    run("ComputerSystemTest", [] {
        ComputerSystemFactory computerSystemFactory;
        ComputerSystemTest computerSystemTest;
        computerSystemTest.TestSystems(computerSystemFactory);
    });

    run("EquateTest", [] { EquateTest::TestEquate(); });

    run("DisassemblyResultTest", [] { DisassemblyResultTest::TestDisassemblyResult(); });

    run("SegmentTest", [] { SegmentTest::TestSegment(); });

    std::printf("\n%s\n", failed == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    return failed == 0 ? 0 : 1;
}
