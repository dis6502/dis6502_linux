#pragma once

#include "Syntax.h"

#include "DefaultFoldersLogic.h"
#include "systems/ComputerSystemFactory.h"
#include "ProfileLogic.h"
#include "FileSystemLogic.h"
#include "EquateListLogic.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"
#include "Console.h"
#include "TestUnit.h"

enum class FileType;
enum class ComputerSystemType;

class MainTest {

public:

    enum class TestMode {
        UNDEFINED, DEV, FAST, NORMAL, DEEP
    };

    MainTest();
    bool Execute(wstring_view testModeName); // Return true is the main programm shall be started, too

    static wstring GetSuitePath();

private:
    // Number of variant bits used in ExecuteUnitTestItem
    static constexpr int VARIANT_BITS = 4;

    Console console;
    int totalCount;
    int failedCount;
    unsigned int variant;

    // See SetupUnit()
    TestUnit testUnit;

    // See Setup()
    std::unique_ptr<FileSystemLogic> fileSystemLogic = nullptr;
    std::unique_ptr<ComputerSystemFactory> computerSystemFactory = nullptr;
    std::unique_ptr<DefaultFoldersLogic> defaultFoldersLogic = nullptr;
    std::unique_ptr<ProfileLogic> profileLogic = nullptr;
    std::unique_ptr<WorkspaceLogic> workspaceLogic = nullptr;
    std::unique_ptr<Workspace> workspace = nullptr;
    std::unique_ptr<EquateListLogic> equateListLogic = nullptr;

    void Log(wstring_view text, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"");


    void Execute(TestMode testMode);

    void SetupUnit(wstring_view areaName, wstring_view unitName);
    wstring GetUnitInFilePath(wstring_view fileName);
    wstring GetUnitRefFilePath(wstring_view fileName);
    wstring GetUnitOutFilePath(wstring_view fileName);

    void Setup();
    void Teardown();


    void TestCommon();
    void TestDiskImage();

    void TestEquate();
    void TestDisassembly();
    void TestSegment();
    void TestWorkspace();

    void ExecuteVariant();
    int System(wstring_view command);

    bool ExecuteUnitTest(wstring_view unitName, FileType fileType, wstring_view fileName, wstring_view outputFileName, wstring_view referenceFileName);
    bool ExecuteUnitTestItem(wstring_view unitName, FileType fileType, wstring_view fileName, wstring_view outputFileName, wstring_view referenceFileName);

};

wstring ToString(MainTest::TestMode testMode);
