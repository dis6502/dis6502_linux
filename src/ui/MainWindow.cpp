#include "Application.h"
#include "DisassemblyWindow.h"
#include "Layout.h"
#include "LogListWindow.h"
#include "MainWindow.h"
#include "MainWindowMenu.h"
#include "MemoryInspectorWindow.h"
#include "SegmentListWindow.h"
#include "StringUtility.h"
#include "XRefListWindow.h"


void MainWindow::RegisterClassFor(HINSTANCE hInstance, WNDPROC lpfnWndProc) {
    /*
    ** Fill in window class structure with parameters that describe the
    ** main window.
    */
    WNDCLASS wc = { };
    wc.style = 0;
    wc.lpfnWndProc = lpfnWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_DIS6502));
    wc.hCursor = LoadCursor(NULL_HINSTANCE, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = L"MAIN_MENU";
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) {
        safeExit(String::wstring_to_ansi(String::Format(L"Cannot register class %s", CLASS_NAME)).c_str());
    }
}

MainWindow::MainWindow(Layout& layout) :Window() {
    this->layout = &layout;

    mainWindowMenu = std::make_unique<MainWindowMenu>();
    segmentListWindow = std::make_unique<SegmentListWindow>(*this, layout.segmentList);
    memoryInspectorWindow = std::make_unique<MemoryInspectorWindow>(*this, layout.memoryInspector);
    disassemblyWindow = std::make_unique<DisassemblyWindow>(*this, layout.disassembly);
    xrefListWindow = std::make_unique<XRefListWindow>(*this, layout.xrefList);
    logListWindow = std::make_unique<LogListWindow>(*this, layout.logList);
}

void MainWindow::CreateControl() {
    Window::CreateWindowControl(CLASS_NAME, L"Main Window",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX,
        0, CW_USEDEFAULT, CW_USEDEFAULT,
        layout->mainWidth, layout->mainHeight);

    mainWindowMenu->CreateControl(hWnd);
    segmentListWindow->CreateControl(ID_SEGMENT_CONTROL);
    memoryInspectorWindow->CreateControl(ID_DUMP_CONTROL);
    disassemblyWindow->CreateControl(ID_DIS_CONTROL);
    xrefListWindow->CreateControl(ID_XREF_CONTROL);
    logListWindow->CreateControl(ID_LOG_CONTROL);
}

void MainWindow::ApplyLayout() {
    segmentListWindow->ApplyLayout();
    memoryInspectorWindow->ApplyLayout();
    disassemblyWindow->ApplyLayout();
    xrefListWindow->ApplyLayout();
    logListWindow->ApplyLayout();

    RedrawWindow(hWnd, nullptr, NULL_HRGN, RDW_INVALIDATE);
}
