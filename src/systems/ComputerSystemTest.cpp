#include "ComputerSystemTest.h"

#include "atari800/Atari800Test.h"
#include "TestPaths.h"
#include "ComputerSystemFactory.h"
#include "ComputerSystem.h"

void ComputerSystemTest::TestSystems(const ComputerSystemFactory& computerSystemFactory) {
    TestAtari800(*computerSystemFactory.GetComputerSystem(ComputerSystemType::ATARI800));
    TestC64(*computerSystemFactory.GetComputerSystem(ComputerSystemType::C64));

}

void ComputerSystemTest::SetupUnit(wstring_view areaName, wstring_view unitName) {
    testUnit = TestUnit(TestPaths::GetSuitePath(), areaName, unitName);
}

void ComputerSystemTest::TestAtari800(const ComputerSystem& computerSystem) {

    SetupUnit(L"system", L"atari800");

    TestReadFile(computerSystem, L"Segments-RUNAD.xex");
    TestReadFile(computerSystem, L"Segments-SillyThings.xex");
    //TestReadFile(computerSystem, L"fa.com"); // TODO: SDX binary, fails in export comparison

    TestReadDiskImageFile(computerSystem, L"Segments.atr", L"ATADOS25.COM", true);  // Broken DOS headder
    TestReadDiskImageFile(computerSystem, L"Segments.atr", L"TRANSDIS.COM", true);  // Broken segment structure
    TestReadDiskImageFile(computerSystem, L"Segments.atr", L"BLAKDISK.COM", false); // Correct XEX file
    TestReadDiskImageFile(computerSystem, L"Segments.atr", L"BLAKDISK.COM", false); // Correct XEX file
    TestReadDiskImageFile(computerSystem, L"DOS_25_ED.atr", L"COPY32.COM", false);  // Correct XEX file with intermediate $FFFF header
    TestReadDiskImageFile(computerSystem, L"DOS_25_ED.atr", L"DISKFIX.COM", false); // Correct XEX file with intermediate $FFFF header

    //EXE: Loading program 0006 - 003C to 7000 - 7036
    //EXE : Loading program 0041 - 013C to 5000 - 50FB
    //EXE : Loading program 0141 - 023C to 50FC - 51F7
    //EXE : Loading program 0241 - 033C to 51F8 - 52F3
    //EXE : Loading program 0341 - 043C to 52F4 - 53EF
    //EXE : Loading program 0441 - 053C to 53F0 - 54EB
    //EXE : Loading program 0541 - 063C to 54EC - 55E7
    //EXE : Loading program 0641 - 073C to 55E8 - 56E3
    //EXE : Loading program 0741 - 083C to 56E4 - 57DF
    //EXE : Loading program 0841 - 0852 to 57E0 - 57F1
    //EXE : Loading program 0857 - 0858 to 02E2 - 02E3
    //EXE: Jumping to 5000

}

void ComputerSystemTest::TestC64(const ComputerSystem& computerSystem) {

    SetupUnit(L"system", L"c64");
    TestReadFile(computerSystem, L"HelloWorld.prg");

}

void ComputerSystemTest::TestReadFile(const ComputerSystem& computerSystem, wstring_view fileName) {
    auto filePath = testUnit.GetInFilePath(fileName);
    auto outFilePath = testUnit.GetOutFilePath(fileName);

    Atari800Test::AssertSegmentListEquals(computerSystem, FileType::EXECUTABLE_FILE, filePath, outFilePath);
}


void ComputerSystemTest::TestReadDiskImageFile(const ComputerSystem& computerSystem, wstring_view diskImageFileName, wstring_view fileName, bool expectReadFailure) {
    auto diskImageFilePath = testUnit.GetInFilePath(diskImageFileName);
    auto outFilePath = testUnit.GetOutFilePath(diskImageFileName) + L".files" + FileIO::FILE_SEPARATOR + wstring(fileName);

    Atari800Test::AssertSegmentListEquals(computerSystem, FileType::EXECUTABLE_FILE, diskImageFilePath, fileName, expectReadFailure, outFilePath);
}
