
#include "Application.h"
#include "DC.h"
#include "DefaultFoldersDialog.h"
#include "DefaultFoldersLogic.h"
#include "DisassemblyControl.h"
#include "DisassemblyWindow.h"
#include "EquateList.h"
#include "EquateListController.h"
#include "EquateListLogic.h"
#include "FileDialogs.h"
#include "FileType.h"
#include "Layout.h"
#include "Main.h"
#include "MainDisassembly.h"
#include "MainFile.h"
#include "MainMemoryInspector.h"
#include "MainMenu.h"
#include "MainSegment.h"
#include "MainUITest.h"
#include "MainWindow.h"
#include "MainWindowMenu.h"
#include "MainXRef.h"
#include "Memory.h"
#include "MemoryInspector.h"
#include "MemoryInspectorControl.h"
#include "MemoryInspectorSelection.h"
#include "MemoryInspectorWindow.h"
#include "MessageBoxDialog.h"
#include "MRUController.h"
#include "PartWindow.h"
#include "Profile.h"
#include "ProfileDialog.h"
#include "ProfileLogic.h"
#include "ProfilesController.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SpriteControl.h"
#include "StringUtility.h"
#include "systems/ComputerSystem.h"
#include "systems/ComputerSystemFactory.h"
#include "systems/ComputerSystemType.h"
#include "Text.h"
#include "UI.h"
#include "Workspace.h"
#include "WorkspaceFont.h"
#include "WorkspaceLogic.h"
#include "WorkspaceTypes.h"
#include <exception>
#include <filesystem>
#include <gsl/pointers>
#include <iostream>
#include <locale>
#include <malloc.h>
#include <memory>
#include <Resource.h>
#include <string>
#include <strsafe.h>
#include <Syntax.h>
#include <vector>
#include <wchar.h>
#include <Windows.h>

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

// Main Globals
extern wstring binPath; // Part of workspace. Defined in Workspace1X.cpp, used by Main/MainFile and Workspace Logic
extern wstring diskPath; // Part of workspace. Defined in Workspace1X.cpp, used by Main/MainFile and Workspace Logic

std::unique_ptr<Main> g_Main = nullptr;
std::unique_ptr<Application> g_Application = nullptr;
std::unique_ptr<Workspace> g_Workspace = nullptr;

std::unique_ptr<MemoryInspector> g_MemoryInspector = nullptr; // TODO SHould be MainMemoryInspector & Workspace

std::unique_ptr<FileDialogs> g_FileDialogs = nullptr;
std::unique_ptr<EquateListController> g_EquateListController = nullptr;


Main::~Main() {

}

void Main::SetMainWindowTitle() {
    const auto filePath = ::g_Workspace->GetFilePath();
    const auto& computerSystemText = ::g_Workspace->GetComputerSystem()->GetTypeInfo()->text;
    wstring title;
    if (filePath.empty()) {
        title = Text::Format(IDS_MAIN_WINDOW_TITLE_NO_WORKSPACE_LOADED, computerSystemText);
    }
    else {
        title = Text::Format(IDS_MAIN_WINDOW_TITLE, computerSystemText, filePath);
    }

    mainWindow->SetTitle(title);
}

void Main::OpenFileWithDialog(const FileType fileType, bool add) {
    mainFile->OpenFileWithDialog(*mainWindow, fileType, add);
}

void Main::OpenFile(wstring_view filePath, const FileType fileType, bool add) {
    mainFile->OpenFile(*mainWindow, filePath, fileType, add);
}

void Main::OpenWorkspaceFile(wstring_view filePath) {
    if (PromptToClearWorkspace(IDS_MAIN_FILE_OPEN_WORKSPACE_FILE_TITLE, IDS_MAIN_FILE_OPEN_WORKSPACE_FILE_MESSAGE, false)) {
        workspaceLogic->Load(*g_Workspace, filePath);
    }
}

void Main::ShowProfileDialog() {
    ProfileDialog dialog(*mainWindow, *profilesController);

    const auto result = dialog.Show(*::g_Workspace->GetProfile(), *::g_Workspace->GetComputerSystem()->GetTypeInfo());

    if (result) {
        ::g_Workspace->NotifyProfileChanged();
    }
}

void Main::ShowDefaultFoldersDialog() {
    DefaultFoldersDialog dialog(*mainWindow);

    auto p_df = defaultFolders.get();
    if (!p_df) {
        defaultFolders = defaultFoldersLogic->CreateDefaultFolders(*::g_Workspace->GetComputerSystem()->GetTypeInfo());

        defaultFoldersLogic->Load(*defaultFolders);
        ::g_FileDialogs->SetDefaultFolders(*defaultFolders);
    }

    dialog.Show(hMainWnd, *defaultFolders);
}

void  Main::ToggleViewDoubleFontHeight() {
    ::g_Workspace->SetViewDoubleHeight(!::g_Workspace->IsViewDoubleHeight());
    ::g_Workspace->NotifyFontChanged();
};

HFONT Main::GetResizedFont() {
    return WorkspaceFont::GetResizedFont(*g_Workspace);
}

void Main::SetLayoutFont() {
    layout->SetFont(GetResizedFont(), WorkspaceFont::GetResizedFontWidth(*g_Workspace), WorkspaceFont::GetResizedFontHeight(*g_Workspace));
}

MainDisassembly* Main::GetMainDisassembly() {
    return mainDisassembly.get();
}

void Main::UpdateDisassembly(bool bForce) {
    if (::g_Workspace->GetComputerSystem()->GetType() == ComputerSystemType::UNKNOWN) {
        return;
    }

    if (bForce || !::g_Workspace->isViewNoDisassembly()) {
        ::g_MemoryInspector->Disassemble();
    }
}

void  Main::ToggleViewDisassembly() {
    ::g_Workspace->SetViewNoDisassembly(!::g_Workspace->isViewNoDisassembly());

    UpdateDisassembly();	// Will do nothing is case it is now true
}

void Main::SaveDisassemblyFiles() {
    mainFile->SaveDisassemblyFiles(*mainWindow);
}

void Main::HandleWorkspaceChanged(const Workspace& workspace, const std::vector<WorkspaceProperty>& propertyChangeEvents) {

    bool updateEquates = false;
    bool updateSegments = false;
    bool updateDisassembly = false;
    bool updateDisassemblyForce = false;

    for (auto property : propertyChangeEvents) {
        switch (property) {
        case WorkspaceProperty::COMPUTER_SYSTEM_TYPE:
            SetMainWindowTitle();

            if (defaultFolders) {
                defaultFoldersLogic->Save(*defaultFolders);
                defaultFolders.reset();
            }

            defaultFolders = defaultFoldersLogic->CreateDefaultFolders(*workspace.GetComputerSystem()->GetTypeInfo());
            defaultFoldersLogic->Load(*defaultFolders);
            ::g_FileDialogs->SetDefaultFolders(*defaultFolders);
            workspaceLogic->LoadSystemEquates(*::g_Workspace);
            break;

        case WorkspaceProperty::FONT:
            SetLayoutFont();
            layout->Compute();
            mainWindow->ApplyLayout();
            break;

        case WorkspaceProperty::FILE_PATH:
            SetMainWindowTitle();
            break;

        case WorkspaceProperty::PROFILE:
            updateDisassembly = true;
            updateDisassemblyForce = true;
            break;

        case WorkspaceProperty::SYSTEM_EQUATES:
        case WorkspaceProperty::USER_EQUATES:
            updateEquates = true;
            updateDisassembly = true;
            break;

        case WorkspaceProperty::SEGMENTS:
            updateSegments = true;
            updateDisassembly = true;
            updateDisassemblyForce = true;
            break;

        case WorkspaceProperty::SELECTED_SEGMENT: {
            updateSegments = true;
            break;
        }

        default:
            break;
        }
    }

    if (updateEquates) {
        // TODO: Conflict with const definition of workspace
        // workspace.GetDisassemblyResult()->ResetEquateSections();
    }

    if (updateSegments) {
        mainSegment->UpdateList();
        const auto& segmentList = workspace.GetConstSegmentList();
        mainWindow->segmentListWindow->SetFocus();
        const auto& memoryInspectorSelection = g_MemoryInspector->GetMemoryInspectorSelection();
        if (segmentList->GetSelectedIndex() != memoryInspectorSelection->GetSegmentIndex()) {
            memoryInspectorSelection->SetSegmentIndex(segmentList->GetSelectedIndex());

            ::g_MemoryInspector->SegmentChanged();
            //::g_MemoryInspector->Select(0, 1);

            // TODO: Selecting a line in the disassembly before doing the disassembly makes no sense
            //const auto segment = ::g_MemoryInspector->GetConstMemoryInspectorSelection()->GetConstSegment();
            //if (segment == nullptr) {
            //    mainDisassembly->SelectLine(0);
            //}
            //else {
            //    mainDisassembly->SelectLine(segment->GetFirstLineNumber());
            //}

            RedrawMainWindow();
        }
    }

    if (updateDisassembly) {
        UpdateDisassembly(updateDisassemblyForce);
    }
}

bool Main::SaveWorkspaceFile(bool bSaveAs) {
    return mainFile->SaveWorkspaceFile(*mainWindow, bSaveAs);
}

void Main::ClearWorkspace() {
    mainMemoryInspector->QuitEditMode();

    // Clear workspace content (file path and segments).
    ::g_Workspace->BeginUpdate();
    ::g_Workspace->SetFilePath(L"");
    ::g_Workspace->GetSegmentList()->Clear();
    ::g_Workspace->GetDisassemblyResult()->Clear();
    ::g_Workspace->EndUpdate();

    ::g_MemoryInspector->Reset();

    RedrawMainWindow();
}

bool Main::PromptToClearWorkspace(Text::TextID titleTextID, Text::TextID messageTextID, bool loadSystemEquates) {
    if (::g_Workspace->GetConstSegmentList()->IsEmpty()) {
        return true;
    }

    auto title = Text::Get(titleTextID);
    auto message = Text::Get(messageTextID);

    const auto result = MessageBoxDialog::ShowQuestion(mainWindow.get(), title, message, MessageBoxDialog::ButtonType::YESNOCANCEL);
    if (result == MessageBoxDialog::ButtonID::CANCEL) {
        return false;
    }

    if (result == MessageBoxDialog::ButtonID::YES) {
        if (!mainFile->SaveWorkspaceFile(*mainWindow, false)) {
            return false;
        }
    }

    ClearWorkspace();
    if (loadSystemEquates) {
        workspaceLogic->LoadSystemEquates(*::g_Workspace);
    }

    return true;
}

void Main::RedrawMainWindow() const {
    InvalidateRect(hMainWnd, nullptr, false);
}

void Main::PaintMainWindow() {

    auto hWnd = hMainWnd;
    const auto fontHeight = layout->fontHeight;

    // Set computer font as active font.
    PAINTSTRUCT ps = {};
    auto hDC = BeginPaint(hWnd, &ps);
    DC dc(hDC);
    auto hFont = GetResizedFont();
    auto hOldFont = (HFONT)SelectObject(hDC, hFont);
    const auto rgbOldTextColor = dc.SetTextColor(RGB(0, 0, 0));

    // Print segments window title.
    const auto rgbOldBkColor = dc.SetBkColor(RGB(255, 255, 0));

    wstring title;
    if (::g_Workspace->GetConstSegmentList()->IsEmpty()) {
        title = Text::Get(IDS_SEGMENT_TITLE_NO_SEGMENTS_LOADED);
    }
    else {
        title = Text::Format(IDS_SEGMENT_TITLE, std::filesystem::path(binPath).filename().wstring());
    }

    RECT rcMain = {};
    GetClientRect(hWnd, &rcMain);
    POINT leftTop = {};
    leftTop.x = rcMain.left;
    leftTop.y = rcMain.top;
    ClientToScreen(hWnd, &leftTop);
    rcMain.left = leftTop.x;
    rcMain.top = leftTop.y;

    RECT rcChild = {};
    GetWindowRect(mainWindow->segmentListWindow->GetHWnd(), &rcChild);
    RECT rc = {};
    rc.right = rcChild.right - rcChild.left;
    rc.left = 0;
    rc.bottom = rcChild.top - rcMain.top;
    rc.top = 0;
    dc.ExtTextOut(1, 1, rc, title);

    // Print disassembly window title.
    dc.SetBkColor(RGB(0, 255, 0));
    rc.left = rc.right;
    rc.right = rcMain.right;
    dc.ExtTextOut(rc.left + 1, rc.top + 1, rc, Text::Get(IDS_DIS_TITLE));
    rc.right = rc.left;
    rc.left = 0;

    // Print memory memory inspector window title.
    // TODO Detection of focus does not actually work.
    dc.SetTextColor(mainWindow->memoryInspectorWindow->GetMemoryInspectorControl()->HasFocus() ? RGB(0, 0, 0) : RGB(128, 128, 128));
    dc.SetBkColor(RGB(0, 255, 255));
    const auto& memoryInspectorSelection = ::g_MemoryInspector->GetConstMemoryInspectorSelection();
    const auto& segment = memoryInspectorSelection->GetConstSegment();

    if (segment == nullptr) {
        title = Text::Get(IDS_DUMP_TITLE_SEGMENT_NO_SEGMENT_SELECTED);
    }
    else {
        const Memory::address wBegin = segment->wBegin;
        const Memory::address wEnd = segment->wEnd;
        Memory::size size;

        if (memoryInspectorSelection->IsEmpty()) {
            size = segment->GetSize();
            // Segment {0}: ${1}-${2}:${3} / {4}
            title = Text::Format(IDS_DUMP_TITLE_SEGMENT,
                std::to_wstring(memoryInspectorSelection->GetSegmentIndex() + 1),
                Memory::address_to_hex_string(wBegin), Memory::address_to_hex_string(wEnd),
                Memory::size_to_hex_string(size), Memory::size_to_string(size));
        }
        else {
            size = memoryInspectorSelection->GetSize();
            // Selection{ 0 }: ${ 1 } - ${2} : ${ 3 } / {4}
            title = Text::Format(IDS_DUMP_TITLE_SELECTION,
                std::to_wstring(memoryInspectorSelection->GetSegmentIndex() + 1),
                Memory::address_to_hex_string(wBegin + memoryInspectorSelection->GetBegin()),
                Memory::address_to_hex_string(wBegin + memoryInspectorSelection->GetEnd()),
                Memory::size_to_hex_string(size), Memory::size_to_string(size));
        }
    }
    rc.top = rc.bottom + rcChild.bottom - rcChild.top;
    rc.bottom += rc.top;
    dc.ExtTextOut(1, rc.top + 1, rc, title); // TODO: Doesn not work

    // Print xref window title.
    dc.SetBkColor(RGB(255, 192, 192));
    rc.right = rcMain.right;
    rc.left = rcChild.right - rcChild.left;
    rc.bottom = layout->xrefList.top;
    rc.top = rc.bottom - fontHeight - 2;
    dc.ExtTextOut(rc.left + 1, rc.top + 1, rc, mainWindow->xrefListWindow->GetTitle());

    // Print log list title.
    dc.SetBkColor(RGB(192, 192, 255));
    rc.right = rcMain.right;
    rc.left = 0;
    rc.bottom = layout->logList.top;
    rc.top = rc.bottom - fontHeight - 2;
    dc.ExtTextOut(rc.left + 1, rc.top + 1, rc, Text::Get(IDS_LOG_TITLE));

    // Restore context.
    SetTextColor(hDC, rgbOldTextColor);
    SetBkColor(hDC, rgbOldBkColor);
    SelectObject(hDC, hOldFont);
    EndPaint(hWnd, &ps);
}

void Main::CreateControls() {
    // Register all classes once.
    auto hInstance = ::g_Application->GetInstanceHandle();
    MainWindow::RegisterClassFor(hInstance, MainWndProc);
    MemoryInspectorControl::RegisterClassFor(hInstance);
    SpriteControl::RegisterClassFor(hInstance);
    DisassemblyControl::RegisterClassFor(hInstance);

    // Create a main window for this application instance.
    mainWindow = std::make_unique<MainWindow>(*layout.get());
    mainWindow->CreateControl();
    hMainWnd = mainWindow->GetHWnd();
    mainWindow->SetMouseWheelProc(MainMouseWheelProc);

    // Setup segment listbox.
    mainWindow->segmentListWindow->SetDropFilesProc(MainSegment::DropFilesProc);
    mainWindow->segmentListWindow->SetRButtonDownProc(MainSegment::RButtonDownProc);
    mainWindow->segmentListWindow->SetMouseWheelProc(MainMouseWheelProc);
    mainWindow->segmentListWindow->SetDragAcceptFiles(true);

    // Setup memory inspector window.
    ::g_MemoryInspector->CreateControl();

    // Setup xref listbox.
    mainWindow->xrefListWindow->SetMouseWheelProc(MainMouseWheelProc);

    // Setup log listbox.
    mainWindow->logListWindow->SetMouseWheelProc(MainMouseWheelProc);

    ::g_Application->SetLogListWindow(mainWindow->logListWindow.get());
}

bool Main::InitApplication(HINSTANCE hInstance, wstring& commandLine, int nCmdShow) {
    ::g_Application = std::make_unique<Application>(hInstance, L"dis6502.ini");

    // Create not workspace related logics.
    defaultFoldersLogic = std::make_unique<DefaultFoldersLogic>();
    computerSystemFactory = std::make_unique<ComputerSystemFactory>();

    // Parse computer name from command line (/ATARI800, /ATARI5200, /ORIC,...)
    // After the type parameter, a file name can be specified and ids returned in the command line parameter per reference
    wstring computerSystemTypeID;
    if (commandLine.starts_with(L"/")) {
        commandLine = commandLine.substr(1);
        const auto index = commandLine.find_first_of(' ');
        if (index == wstring::npos) {
            computerSystemTypeID = commandLine;
            commandLine = L"";
        }
        else {
            computerSystemTypeID = commandLine.substr(0, index);
            commandLine = commandLine.substr(index + 1);
        }
    }

    // Execute unit tests?
    bool testMode = false;
    if (String::StartsWith(computerSystemTypeID, L"TEST")) {
        const wstring testModeParameter = computerSystemTypeID;
        wstring testModeName = L"";
        if (String::StartsWith(testModeParameter, L"TEST:")) {
            testMode = true;
            testModeName = testModeParameter.substr(5);
        }
        MainUITest mainUITest;
        if (!mainUITest.Execute(testModeName)) {
            return false;
        }
    }

    // Create empty workspace.
    profileLogic = std::make_unique<ProfileLogic>();
    equateListLogic = std::make_unique<EquateListLogic>();
    workspaceLogic = std::make_unique<WorkspaceLogic>(*equateListLogic);
    ::g_Workspace = std::make_unique<Workspace>(*computerSystemFactory);

    ::g_MemoryInspector = std::make_unique<MemoryInspector>();

    mruController = std::make_unique<MRUController>();

    // Prepare initial layout.
    layout = std::make_unique<Layout>();
    SetLayoutFont();
    layout->ComputeForSize(1024, 768);

    CreateControls();

    ::g_EquateListController = std::make_unique<EquateListController>(*equateListLogic);
    g_EquateListController->SetParentWindow(*mainWindow);

    ::g_FileDialogs = std::make_unique<FileDialogs>();
    profilesController = std::make_unique<ProfilesController>(*profileLogic);
    mainFile = std::make_unique<MainFile>(*this, *mruController, *workspaceLogic);
    mainMenu_ = std::make_unique<MainMenu>(*this, *mruController);
    mainSegment = std::make_unique<MainSegment>(*this);
    mainMemoryInspector = std::make_unique<MainMemoryInspector>(*this);
    mainDisassembly = std::make_unique<MainDisassembly>(*this);
    mainXRef = std::make_unique<MainXRef>(*this);

    // Mutual event handler registration
    mainDisassembly->SetControl();
    mainDisassembly->SetMainXRef(*mainXRef);
    mainXRef->SetMainDisassembly(*mainDisassembly);

    // Load main menu accelerators.
    if (!(hAccelerators = LoadAccelerators(hInstance, L"Accelerators"))) {
        return false;
    }

    ::g_MemoryInspector->Init(*mainWindow, *mainWindow->memoryInspectorWindow);

    // Add "L" prefix to compile const char* constants as const wchar_t.
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
    ::g_Application->SendInfoMessageWithID(IDS_LOG_BETA_MESSAGE, WIDE1(__DATE__), WIDE1(__TIME__));

    ::g_Workspace->AddListener(*this);

    ::g_Workspace->SetComputerSystemTypeID(computerSystemTypeID);

    binPath = L"";
    diskPath = L"";
    ::g_Workspace->SetFilePath(L"");
    // Using line numbers in test mode simplifies debugging
    if (testMode) {
        g_Workspace->GetProfile()->useLineNumbers = true;
    }

    // Make the window visible; update its client area; and return "success".
    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    return true;
}

void Main::ExitApplication() {


    if (::g_FileDialogs != nullptr) {
        ::g_FileDialogs.reset();
    }

    // Reset all global instances outside of the application scope.
    if (::g_EquateListController != nullptr) {
        ::g_EquateListController.reset();
    }

    ::g_MemoryInspector.reset();

    if (::g_Workspace != nullptr) {
        ::g_Workspace->RemoveListeners();
        ::g_Workspace.reset();
    }

    // Release all instances inside the application scope.
    ::g_Application.reset();
}

MainWindow* Main::GetMainWindow() {
    return mainWindow.get();
}

int Main::WinMainDelegate(HINSTANCE hInstance, const HINSTANCE hPrevInstance, wstring_view commandLine, int nCmdShow) {
    // Finding Memory Leaks Using the CRT Library.
    // See https://msdn.microsoft.com/en-us/library/x98tx3cf.aspx
    if (commandLine.starts_with(L"/DEBUG")) { // Must be the first parameter
#ifdef _CRTDBG_MAP_ALLOC
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_LEAK_CHECK_DF);
#else
        MessageBoxDialog::Show(nullptr, L"Command Line Option /DEBUG Ignored", L"This version was compiled without define _CRTDBG_MAP_ALLOC. Debugging memory is not possible.");
#endif 

    }

    // TODO: Set allcation ID where the program should break
    //_CrtSetBreakAlloc(200);

    /*
    ** if the program is already running, we activate the previous
    ** instance and close this one. Otherwise settings in INI file
    ** would result in concurrent overwrites.
    */
    if (hPrevInstance) {
        auto hWndPrevApp = FindWindow(MainWindow::CLASS_NAME, nullptr);
        if (IsWindow(hWndPrevApp)) {
            BringWindowToTop(hWndPrevApp);
            return 0;
        }
    }
    MSG msg = {};
    msg.wParam = 0;

    auto commandLineString = wstring(commandLine);
    if (InitApplication(hInstance, commandLineString, nCmdShow)) {
        // populate recent files and recent workspace menu items
        mruController->Load();

        // if a file has been given in the command line, we handle it.
        if (!commandLine.empty()) {
            auto filePath = std::filesystem::absolute(std::filesystem::path(commandLine)).wstring();
            auto filePathCopy = _wcsdup(filePath.c_str());
            PostMessage(hMainWnd, WM_OPENCMDLINE, 0, (LPARAM)filePathCopy);
        }

        // Acquire and dispatch messages until a WM_QUIT message is received.
        while (GetMessage(&msg, NULL_HWND, 0, 0)) {
            if (!TranslateAccelerator(hMainWnd, hAccelerators, &msg)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // free resources
        ClearWorkspace();

        mruController->Save();
        if (defaultFolders != nullptr) {
            defaultFoldersLogic->Save(*defaultFolders);
        }
    }

    ExitApplication();

    return msg.wParam;
}

LRESULT Main::MainWndProcDelegate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        PaintMainWindow();
        return 0;

    case WM_INITMENU: {
        const auto mainWindowMenu = mainWindow->mainWindowMenu.get();
        const auto editMode = ::g_MemoryInspector->IsEditMode();
        const auto& computerSystem = ::g_Workspace->GetComputerSystem();

        mainWindowMenu->SetEnabled(ID_FILE_OPEN_CASSETTE_IMAGE_FILE, computerSystem->IsSupportedFileType(FileType::CASSETTE_IMAGE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_OPEN_DISK_IMAGE_BOOT_SECTORS, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_BOOT_SECTORS) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_OPEN_DISK_IMAGE_EXECUTABLE_FILE, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_EXECUTABLE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_OPEN_DISK_IMAGE_SECTORS, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_SECTORS) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_OPEN_EXECUTABLE_FILE, computerSystem->IsSupportedFileType(FileType::EXECUTABLE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_OPEN_ROM_IMAGE_FILE, computerSystem->IsSupportedFileType(FileType::ROM_IMAGE_FILE) && !editMode);

        mainWindowMenu->SetEnabled(ID_FILE_ADD_CASSETTE_IMAGE_FILE, computerSystem->IsSupportedFileType(FileType::CASSETTE_IMAGE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_ADD_DISK_IMAGE_BOOT_SECTORS, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_BOOT_SECTORS) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_ADD_DISK_IMAGE_EXECUTABLE_FILE, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_EXECUTABLE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_ADD_DISK_IMAGE_SECTORS, computerSystem->IsSupportedFileType(FileType::DISK_IMAGE_SECTORS) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_ADD_EXECUTABLE_FILE, computerSystem->IsSupportedFileType(FileType::EXECUTABLE_FILE) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_ADD_ROM_IMAGE_FILE, computerSystem->IsSupportedFileType(FileType::ROM_IMAGE_FILE) && !editMode);

        mainWindowMenu->SetEnabled(ID_FILE_SAVE_WORKSPACE, (!::g_Workspace->GetConstSegmentList()->IsEmpty()) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_SAVE_WORKSPACE_AS, (!::g_Workspace->GetConstSegmentList()->IsEmpty()) && !editMode);
        mainWindowMenu->SetEnabled(ID_FILE_SAVE_DISK_IMAGE_BOOT_SECTORS, (::g_MemoryInspector->GetConstMemoryInspectorSelection()->HasSegment()) && !editMode);  // TODO Move to segment context menu

        mruController->FillMenu(mainWindowMenu->GetSubMenuHandle(0));

        const auto hasSystemEquates = !::g_Workspace->GetSystemEquateList()->IsEmpty();
        const auto hasUserEquates = !::g_Workspace->GetUserEquateList()->IsEmpty();

        mainWindowMenu->SetEnabled(ID_EQUATES_CLEAR_SYSTEM_EQUATES, hasSystemEquates);
        mainWindowMenu->SetEnabled(ID_EQUATES_CLEAR_USER_EQUATES, hasUserEquates);
        mainWindowMenu->SetEnabled(ID_EQUATES_SAVE_USER_EQUATES, hasUserEquates);
        mainWindowMenu->SetEnabled(ID_EQUATES_EXPORT_USER_EQUATES, hasUserEquates);

        mainWindowMenu->SetChecked(ID_VIEW_DISPLAYASSCREENCODE, ::g_Workspace->IsViewDisplayAsScreenCode());
        mainWindowMenu->SetChecked(ID_VIEW_NODISASSEMBLY, ::g_Workspace->isViewNoDisassembly());
        mainWindowMenu->SetChecked(ID_VIEW_DOUBLE_FONT_HEIGHT, ::g_Workspace->IsViewDoubleHeight());

        return 0;
    }

    case WM_SIZE:
        layout->ComputeForSize(LOWORD(lParam), HIWORD(lParam));
        mainWindow->ApplyLayout();
        return 0;

    case WM_OPENCMDLINE: {
        auto filePathCopy = (LPWSTR)lParam;
        auto filePath = wstring(filePathCopy);
        free(filePathCopy);
        mainFile->OpenFile(*mainWindow, filePath, FileType::UNKNOWN_FILE, false);
        return 0;
    }

    case WM_COMMAND: {
        const auto command = LOWORD(wParam);
        const auto editMode = ::g_MemoryInspector->IsEditMode();

        mainMenu_->PerformCommands(command, editMode);
        mainSegment->PerformCommands(command, editMode);
        mainMemoryInspector->PerformCommands(command, editMode);
        mainDisassembly->PerformCommands(command, editMode);

        // Handle events from the controls.
        // TODO Turn CASE into == lpMainWindow->lpMemoryInspectorWindow->GetHWnd()
        // Or even better into event/proc handlers
        // if ((HWND)wCommand == dis->lpDisassemblyControl->GetHWnd() ) { // TODO verfiy
        //	return mainDisassembly->Proc(hWnd, wParam, lParam);
        //}

        switch (command) {
        case MainWindow::ID_SEGMENT_CONTROL:
            return mainSegment->Proc(hWnd, wParam);
        case MainWindow::ID_DUMP_CONTROL:
            return mainMemoryInspector->Proc(hWnd, wParam);
        case MainWindow::ID_DIS_CONTROL:
            return mainDisassembly->Proc(hWnd, wParam, lParam);
        case MainWindow::ID_XREF_CONTROL:
            return mainXRef->Proc(hWnd, wParam);
        default:
            break;
        }
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

/*
** WM_MOUSEWHEEL message
*/
LRESULT CALLBACK Main::MainMouseWheelProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static short zDelta = 0;

    POINT pt;
    GetCursorPos(&pt);

    HWND hChild = WindowFromPoint(pt);
    const auto& mainWindow = ::g_Main.get()->GetMainWindow();
    if (hChild != mainWindow->segmentListWindow->GetHWnd() &&
        hChild != mainWindow->memoryInspectorWindow->GetMemoryInspectorControl()->GetHWnd() &&
        hChild != mainWindow->disassemblyWindow->GetDisassemblyControl()->GetHWnd() &&
        hChild != mainWindow->xrefListWindow->GetHWnd() &&
        hChild != mainWindow->logListWindow->GetHWnd()) {
        return 0;
    }

    zDelta += (short)HIWORD(wParam);

    while (zDelta / WHEEL_DELTA != 0) {
        for (int i = 0; i < 5; ++i) {
            SendMessage(hChild, WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0);
        }

        zDelta += (zDelta > 0) ? -WHEEL_DELTA : WHEEL_DELTA;
    }

    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    auto hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    if (FAILED(hResult)) {
        std::cout << "Failed to initialize COM library with error " << hResult << "\n";
        return -1;
    }
    std::locale::global(std::locale("")); // Prevent memory leak in setlocale
    ::g_Main = std::make_unique<Main>();
    const auto unicodeCommandLine = GetCommandLine(); // Because lpCmdLine does not support Unicode
    int argc;
    auto argv = CommandLineToArgvW(unicodeCommandLine, &argc);

    if (argv == NULL) {
        std::cout << "Failed to parse command line.\n";
        return -1;
    }

    wstring commandLineParameters;
    for (size_t i = 1; i < argc; i++) {
        commandLineParameters += argv[i];
        if (i < argc) {
            commandLineParameters += L" ";
        }
    }
    LocalFree(argv);

    const auto result = ::g_Main->WinMainDelegate(hInstance, hPrevInstance, commandLineParameters, nCmdShow);
    CoUninitialize();
    return result;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    try {
        return ::g_Main->MainWndProcDelegate(hWnd, message, wParam, lParam);
    }
    catch (const std::exception& e) {
        MessageBoxDialog::ShowAlert(nullptr, L"Exception occurred in MainWndProc", String::utf8_to_wstring(e.what())); // TODO: Have special method for exceptions dialog
        return 0;
    }
}
