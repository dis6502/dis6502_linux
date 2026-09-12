#pragma once

#include "DefaultFolders.h"
#include "DefaultFoldersLogic.h"
#include "EquateListLogic.h"
#include "Font.h"
#include "Layout.h"
#include "MainDisassembly.h"
#include "MainFile.h"
#include "MainMemoryInspector.h"
#include "MainMenu.h"
#include "MainSegment.h"
#include "MainWindow.h"
#include "MainXRef.h"
#include "MRUController.h"
#include "ProfileLogic.h"
#include "ProfilesController.h"
#include "Syntax.h"
#include "systems/ComputerSystemFactory.h"
#include "Text.h"
#include "UI.h"
#include "WorkspaceChangedListener.h"
#include "WorkspaceLogic.h"
#include "WorkspaceTypes.h"
#include <memory>
#include <vector>
#include <Windows.h>

enum class FileType;
class Workspace;

class Main final : private WorkspaceChangedListener {
public:
    Main() = default;
    Main(const Main& a) = delete;			// copy constructor
    Main& operator=(const Main&) = delete;  // copy assignment
    Main(Main&&) = delete;                  // move constructor
    Main& operator=(Main&&) = delete;       // move assignment
    ~Main();

    Font* GetResizedFont();

    void OpenFileWithDialog(const FileType fileType, bool add);
    void OpenFile(wstring_view filePath, const FileType fileType, bool add);
    void OpenWorkspaceFile(wstring_view filePath);

    void ShowProfileDialog();
    void ShowDefaultFoldersDialog();

    void ToggleViewDoubleFontHeight();

    MainDisassembly* GetMainDisassembly();
    void UpdateDisassembly(bool bForce = false);
    void ToggleViewDisassembly();
    void SaveDisassemblyFiles();

    bool SaveWorkspaceFile(bool bSaveAs);
    bool PromptToClearWorkspace(Text::TextID titleTextID, Text::TextID messageTextID, bool loadSystemEquates);

    // These procedures need to be public to enable delegation
    MainWindow* GetMainWindow();

    int WinMainDelegate(HINSTANCE hInstance, const HINSTANCE hPrevInstance, wstring_view commandLine, int nCmdShow);

    Window::LRESULT MainWndProcDelegate(Window::WindowHandle hWnd, Window::MESSAGE message, Window::WPARAM wParam, Window::LPARAM lParam);
    static Window::LRESULT CALLBACK MainMouseWheelProc(Window::WindowHandle hWnd, Window::MESSAGE message, Window::WPARAM wParam, Window::LPARAM lParam);

private:
    static const UINT WM_OPENCMDLINE = WM_USER + 248;

    // Window
    std::unique_ptr <MainWindow> mainWindow = nullptr;
    HWND hMainWnd = NULL_HWND; /// TODO: Remove later

    // Logics
    std::unique_ptr<ComputerSystemFactory> computerSystemFactory = nullptr;
    std::unique_ptr<DefaultFoldersLogic> defaultFoldersLogic = nullptr;
    std::unique_ptr<ProfileLogic> profileLogic = nullptr;
    std::unique_ptr<EquateListLogic> equateListLogic = nullptr;
    std::unique_ptr<WorkspaceLogic> workspaceLogic = nullptr;

    std::unique_ptr<MRUController> mruController = nullptr;
    std::unique_ptr<Layout> layout = nullptr;
    HACCEL hAccelerators = NULL_HACCEL;

    // Controllers.
    std::unique_ptr<ProfilesController> profilesController = nullptr;
    std::unique_ptr<MainMenu> mainMenu_ = nullptr;;
    std::unique_ptr<MainFile> mainFile = nullptr;
    std::unique_ptr<MainSegment> mainSegment = nullptr;;
    std::unique_ptr<MainMemoryInspector> mainMemoryInspector = nullptr;
    std::unique_ptr<MainDisassembly> mainDisassembly = nullptr;
    std::unique_ptr<MainXRef> mainXRef = nullptr;

    // State
    std::unique_ptr<DefaultFolders> defaultFolders = nullptr;

    void SetMainWindowTitle();
    void SetLayoutFont();


    void HandleWorkspaceChanged(const Workspace& workspace, const std::vector<WorkspaceProperty>& propertyChangeEvents) override;

    void ClearWorkspace();

    void RedrawMainWindow() const;
    void PaintMainWindow();

    void CreateControls();
    bool InitApplication(HINSTANCE hInstance, wstring& commandLine, int nCmdShow);
    void ExitApplication();
};
