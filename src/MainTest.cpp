
#include "Application.h"
#include "Assertions.h"
#include "CommonIO.h"
#include "CommonTest.h"
#include "ComputerSystemFactory.h"
#include "DefaultFoldersLogic.h"
#include "Disassembly.h"
#include "DisassemblyProgressMonitor.h"
#include "DisassemblyResultFile.h"
#include "DisassemblyResultFileTest.h"
#include "DisassemblyResultTest.h"
#include "EquateListLogic.h"
#include "EquateTest.h"
#include "File.h"
#include "FileHeader.h"
#include "FileIO.h"
#include "FileIOTest.h"
#include "FileType.h"
#include "MainTest.h"
#include "Memory.h"
#include "MemoryInspectorTest.h"
#include "OperatingSystem.h"
#include "Profile.h"
#include "ProfileLogic.h"
#include "Resource.h"
#include "SegmentList.h"
#include "SegmentTest.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "systems/atari800/AtariDiskImageTest.h"
#include "systems/ComputerSystemTest.h"
#include "systems/ComputerSystemType.h"
#include "TestUnit.h"
#include "Text.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"
#include "Writer.h"
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

using TestMode = MainTest::TestMode;
extern Application* g_Application;

enum VariantBits : unsigned int {
    UseHexNotation = 1,
    ShowNonASCIIChararactersAsBytes = 4
};


TestMode TestModeFromString(wstring_view string) {
    auto testMode = TestMode::UNDEFINED;
    if (String::Equals(string, L"DEV")) {
        testMode = TestMode::DEV;
    }
    else if (String::Equals(string, L"FAST")) {
        testMode = TestMode::FAST;
    }
    else if (string.empty() || String::Equals(string, L"NORMAL")) {
        testMode = TestMode::NORMAL;
    }
    else if (String::Equals(string, L"DEEP")) {
        testMode = TestMode::DEEP;
    }
    return testMode;
}

wstring ToString(TestMode testMode) {
    switch (testMode) {
    case MainTest::TestMode::DEV:
        return L"DEV";
    case MainTest::TestMode::FAST:
        return L"FAST";
    case MainTest::TestMode::NORMAL:
        return L"NORMAL";
    case MainTest::TestMode::DEEP:
        return L"DEEP";
    case MainTest::TestMode::UNDEFINED:
        return L"UNDEFIND";
    }
    throw std::runtime_error("Undefined test mode");
}


MainTest::MainTest(Writer& writer) {
    this->writer = &writer;
    totalCount = 0;
    failedCount = 0;
    variant = 0;
}

void MainTest::Log(wstring_view pattern, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3) {
    writer->Write(String::Format(pattern, v0, v1, v2, v3).c_str());
    writer->Write(L"\n");
}


static void AssertAddressEquals(Memory::address current, Memory::address expected) {
    if (current != expected) {
        throw std::runtime_error("No equal");
    }
}

wstring MainTest::GetSuitePath() {
    return g_Application->GetModuleFilePath(L"../../../tst/suite");
}

void MainTest::SetupUnit(wstring_view areaName, wstring_view unitName) {
    testUnit = TestUnit(GetSuitePath(), areaName, unitName);
}

wstring MainTest::GetUnitInFilePath(wstring_view fileName) {
    return testUnit.GetInFilePath(fileName);
}

wstring MainTest::GetUnitRefFilePath(wstring_view fileName) {
    return testUnit.GetRefFilePath(fileName);
}

wstring MainTest::GetUnitOutFilePath(wstring_view fileName) {
    return testUnit.GetOutFilePath(fileName);
}


void MainTest::Setup() {
    // Logics
    computerSystemFactory = std::make_unique<ComputerSystemFactory>();
    defaultFoldersLogic = std::make_unique<DefaultFoldersLogic>();
    profileLogic = std::make_unique<ProfileLogic>();
    equateListLogic = std::make_unique<EquateListLogic>();
    workspaceLogic = std::make_unique<WorkspaceLogic>(*equateListLogic);
    workspace = std::make_unique<Workspace>(*computerSystemFactory);
}

void MainTest::Teardown() {
    workspace.reset();
    workspaceLogic.reset();
    equateListLogic.reset();
    profileLogic.reset();
    defaultFoldersLogic.reset();
}

void MainTest::TestCommon() {

    CommonTest::TestCommon();
    FileIOTest::TestFileIO();
    MemoryInspectorTest::Test();
}


void MainTest::TestEquate() {
    EquateTest::TestEquate();
    SetupUnit(L"equate", L"default");
    auto filePath = GetUnitInFilePath(L"DISK_NOT_FOUND.atr");
    Setup();
    EquateTest::TestFiles(filePath);
    workspace->GetUserEquateList()->Clear();
    Teardown();
}


void MainTest::TestDisassembly() {
    DisassemblyResultTest::TestDisassemblyResult();
    SetupUnit(L"disassembly", L"result");

    auto outFolder = testUnit.GetOutFile(L"");
    DisassemblyResultFileTest::TestDisassemblyResultFile(outFolder);
}

void MainTest::TestSegment() {
    SetupUnit(L"segment", L"default");
    Setup();

    SegmentTest::TestSegment();

    Teardown();
}

void MainTest::TestWorkspace() {
    SetupUnit(L"workspace", L"default");
    Setup();

    auto filePath = GetUnitInFilePath(L"SynCalc-(1993)-128K-Main.wrk");

    Assert::BoolEquals(workspaceLogic->Load(*workspace, filePath), true);

    Teardown();



    Setup();
    const auto& segmentList = workspace->GetSegmentList();
    const auto& segment = segmentList->InsertSegmentAt(0);
    segment->SetHeader(FileHeader::ATARI_BINARY);
    segment->wBegin = 0x1000;
    segment->wEnd = 0x10ff;
    segment->CreateMemoryBlockFromBeginToEnd();
    segmentList->SetUserComment(0, 0x40, 0x40, L"Comment at 0x1040");
    segmentList->SetUserComment(0, 0xC0, 0x40, L"Comment at 0x10C0");

    const auto& newSegment = segmentList->InsertSegmentAt(1);
    segment->SplitAt(0x80, *newSegment);

    AssertAddressEquals(segment->wBegin, 0x1000);
    AssertAddressEquals(segment->wEnd, 0x107f);
    Assert::StringEquals(segment->FindComment(0x40), L"Comment at 0x1040");

    AssertAddressEquals(newSegment->wBegin, 0x1080);
    AssertAddressEquals(newSegment->wEnd, 0x10ff);
    Assert::StringEquals(newSegment->FindComment(0x40), L"Comment at 0x10C0");
    Teardown();
}

void MainTest::TestDiskImage() {

    SetupUnit(L"diskimage", L"ataridiskimage");

    AtariDiskImageTest::AssertFileNames83Equal();

    auto filePath = GetUnitInFilePath(L"DISK_NOT_FOUND.atr");
    AtariDiskImageTest::AssertDirectoryEquals(filePath, L"DISK_NOT_FOUND\n");
    filePath = GetUnitInFilePath(L"DOS_20S_SD.atr");
    AtariDiskImageTest::AssertDirectoryEquals(filePath, L"  DOS     .SYS 039\n  DUP     .SYS 042\n  AUTORUN .SYS 001\nNO_ENTRY_FOUND\n");
    filePath = GetUnitInFilePath(L"DOS_25_SD.atr");
    AtariDiskImageTest::AssertDirectoryEquals(filePath, L"  DOS     .SYS 037\n  DUP     .SYS 042\nNO_ENTRY_FOUND\n");
    filePath = GetUnitInFilePath(L"DOS_25_ED.atr");
    AtariDiskImageTest::AssertDirectoryEquals(filePath, L"* DOS     .SYS 037\n* DUP     .SYS 042\n* RAMDISK .COM 009\n* SETUP   .COM 070\n* COPY32  .COM 056\n* DISKFIX .COM 057\nNO_ENTRY_FOUND\n");
}

void MainTest::Execute(TestMode testMode) {
    Log(L"INFO: Starting unit tests in mode {0}.", ToString(testMode));

    wstring text;
    text = Text::Get(IDS_FILE_SYSTEM_LOGIC_SAVE_FILE_TITLE);
    text = Text::Format(IDS_FILE_SYSTEM_LOGIC_SAVE_FILE_TITLE, L"Test");

    totalCount = 0;
    failedCount = 0;

    try {
        TestCommon();

        TestDiskImage();

        ComputerSystemFactory computerSystemFactory;
        ComputerSystemTest computerSystemTest;
        computerSystemTest.TestSystems(computerSystemFactory);

        TestEquate();

        TestDisassembly();

        TestSegment();

        TestWorkspace();
    }
    catch (const std::exception& ex) {
        Assert::Log(ex);
        safeExit("MainTest::Execute");
    }

    switch (testMode) {

    case TestMode::DEV:
    case TestMode::FAST:
        variant = VariantBits::UseHexNotation | VariantBits::ShowNonASCIIChararactersAsBytes;
        ExecuteVariant();
        break;

    case TestMode::NORMAL: {
        for (int bit = 0; bit < VARIANT_BITS; bit++) {
            variant = 1 << bit;
            ExecuteVariant();
        }
        break;
    }

    case TestMode::DEEP: {
        for (variant = 0; variant < (1 << VARIANT_BITS); variant++) {
            ExecuteVariant();
        }
        break;
    }
    default:
        throw std::runtime_error("Undefined test mode");
    }

    if (failedCount == 0) {
        Log(L"INFO: All {0} unit tests in test mode {1} were successful.", std::to_wstring(totalCount), ToString(testMode));
    }
    else {
        Log(L"ERROR: {0} of {1} unit tests in test mode {2} failed.", std::to_wstring(failedCount), std::to_wstring(totalCount), ToString(testMode));
    }
}

void MainTest::ExecuteVariant() {
    Log(L"INFO: Starting unit test variant {0}.", std::to_hex_string(variant));

    ExecuteUnitTest(L"unit001", FileType::EXECUTABLE_FILE, L"autorun.xex", L"unit001.xex", L"autorun.xex");
    ExecuteUnitTest(L"unit002", FileType::EXECUTABLE_FILE, L"multisegment.xex", L"unit002.xex", L"multisegment.xex");
    ExecuteUnitTest(L"unit003", FileType::WORKSPACE_FILE, L"autorun.wrk", L"unit003.xex", L"autorun.xex");
    ExecuteUnitTest(L"unit004", FileType::WORKSPACE_FILE, L"predux-220810.wrk", L"unit004.xex", L"predux-220810.xex");
    // ExecuteUnitTest(L"unit005", FileType::EXECUTABLE_FILE, L"fa.com", L"unit004.xex", L"fa.com"); // TODO SDX does not work yet because of using $0000 as relcoation base address. This lead to conflict with ZP addressing.

}


int MainTest::ExecuteCommand(wstring_view command) {

    const auto result = OperatingSystem::ExecuteCommand(command);
    Log(L"INFO: Execution of '{0}' ended with return code {1}.", command, std::to_wstring(result));
    return result;
}

bool MainTest::ExecuteUnitTest(wstring_view unitName, FileType fileType, wstring_view fileName, wstring_view outputFileName, wstring_view referenceFileName) {
    Setup();
    const auto result = ExecuteUnitTestItem(unitName, fileType, fileName, outputFileName, referenceFileName);
    Teardown();
    if (result) {
        Log(L"INFO: Unit Test {0} for {1} successful.", unitName, fileName);
    }
    else {
        Log(L"ERROR: Unit Test {0} for {1} failed.", unitName, fileName);
        failedCount++;
    }
    totalCount++;
    return result;
}

bool MainTest::ExecuteUnitTestItem(wstring_view unitName, FileType fileType, wstring_view fileName, wstring_view outputFileName, wstring_view referenceFileName) {


    workspace->SetComputerSystemType(ComputerSystemType::ATARI800);

    Disassembly disAssembly;
    DisassemblyProgressMonitor progressMonitor;
    //progressMonitor.SetVerbose(true); // For testing
    disAssembly.SetWorkspace(*workspace);
    disAssembly.SetProgressMonitor(progressMonitor);

    SetupUnit(L"disassembly", unitName);
    FileIO::SetCurrentWorkingDirectory(GetUnitOutFilePath(L""));

    if (ExecuteCommand(L"del /F/Q *.*") != 0) {
        return false;
    }

    auto inFilePath = GetUnitInFilePath(fileName);

    if (fileType == FileType::WORKSPACE_FILE) {
        if (!workspaceLogic->Load(*workspace, inFilePath)) {
            return false;
        }
        auto inFileEquatesPath = inFilePath + L".equ";
        auto inFileEquatesFile = File(inFileEquatesPath);
        if (inFileEquatesFile.Exists()) {
            if (!workspace->GetUserEquateList()->Load(inFileEquatesFile.GetPath())) {
                return false;
            }

        }
    }
    else {
        if (!workspaceLogic->AddFile(*workspace, fileType, inFilePath)) {
            return false;
        }
    }
    workspaceLogic->LoadSystemEquates(*workspace);

    auto profile = workspace->GetProfile();
    workspace->GetProfile()->omitUnreferencedSystemLabels = false; // TODO Make them work instead

    if (variant & VariantBits::UseHexNotation) {
        Log(L"INFO: Using hex notation");
        profile->useHexNotation = true;
    }
    else {
        Log(L"INFO: Using decimal notation");
        profile->useHexNotation = false;
    }
    if (variant & 0x02) {
        Log(L"INFO: Showing ZP absolute as byte");
        profile->showZPAbsoluteAsByte = true;
    }
    else {
        Log(L"INFO: Showing ZP absolute not as byte");
        profile->showZPAbsoluteAsByte = false;
    }
    if (variant & VariantBits::ShowNonASCIIChararactersAsBytes) {
        Log(L"INFO: Showing non-ASCII characters as bytes");
        profile->showNonASCIIChararactersAsBytes = true;
    }
    else {
        Log(L"INFO: Showing non-ASCII characters as characters");
        profile->showNonASCIIChararactersAsBytes = false;
    }
    if (variant & 0x08) { // TODO Have variant bits
        Log(L"INFO: Showing opcodes as comments");
        profile->showOpcodeAsComment = true;
    }
    else {
        Log(L"INFO: Not showing opcodes as comments");
        profile->showOpcodeAsComment = false;
    }

    const auto duration = progressMonitor.StartDisassembly(disAssembly);
    std::wstringstream stream;
    stream << duration;
    Log(L"INFO: Disassembly completed in {0}", stream.str());

    auto workspaceFileName = wstring(unitName) + L".wrk";
    auto workspaceFilePath = testUnit.GetOutFilePath(workspaceFileName);
    if (!workspaceLogic->Save(*workspace, workspaceFilePath, Workspace::Format::WORKSPACE36)) {
        return false;
    }

    auto disassemblyFileName = wstring(unitName) + L".asm";
    auto disassemblyMainFile = testUnit.GetOutFile(disassemblyFileName);
    DisassemblyResultFile disassemblyResultFile;
    try {
        disassemblyResultFile.SaveListing(*workspace->GetConstDisassemblyResult(), *workspace->GetConstProfile(), disassemblyMainFile);
    }
    catch (const IOException& ex) {
        g_Application->SendErrorMessageWithException(ex);
        return false;
    }

    auto outputFilePath = GetUnitOutFilePath(outputFileName);
    auto outputScriptFilePath = wstring(unitName) + L".bat";
    auto outputLabelFileName = wstring(unitName) + L".lab";
    auto outputListFileName = wstring(unitName) + L".lst";

    auto commandLine = GetSuitePath() + FileIO::FILE_SEPARATOR + L"asm\\mads\\mads.exe " + disassemblyMainFile.GetPath() + L" -o:" + wstring(outputFileName) + L" -t:" + outputLabelFileName + L" -l:" + outputListFileName;
    std::wofstream scriptFile;
    scriptFile.open(FileIO::ToPath(outputScriptFilePath));
    scriptFile << L"@echo off" << std::endl << L"cd \"%~dp0\"" << std::endl << commandLine << std::endl << L"pause" << std::endl;
    scriptFile.close();

    // For programatic execution, outputs are redirected, so they can be loaded
    commandLine += L" >stdout.txt 2>stderr.txt";
    if (ExecuteCommand(commandLine.c_str()) != 0) {
        wstring stdoutFilePath = L"stdout.txt";
        auto text = FileIO::ReadString(stdoutFilePath);
        Log(L"ERROR: Content of {0}\n{1}\n", stdoutFilePath, text);
        wstring stderrFilePath = L"stderr.txt";
        text = FileIO::ReadString(stderrFilePath);
        Log(L"ERROR: Content of {0}\n{1}\n", stderrFilePath, text);
        return false;
    }

    auto refereceFilePath = GetUnitRefFilePath(referenceFileName);
    try {
        FileIOTest::AssertFileEquals(outputFilePath, refereceFilePath);

        // If a disassembly reference file exists for the variant, it is also compared.
        auto disassemblyReferenceFilePath = GetUnitRefFilePath(wstring(unitName) + L"-" + std::to_byte_hex_string(variant) + L".asm");
        if (FileIO::FileExists(disassemblyReferenceFilePath)) {
            FileIOTest::AssertFileEquals(outputFilePath, disassemblyReferenceFilePath);
        }

    }
    catch (const AssertionError&) {
        return false;
    }
    return true;

}

