#pragma once

#include "Syntax.h"

#include "Window.h"
#include "MainWindowMenu.h"

class Layout;
class Main;

#include "SegmentListWindow.h"
#include "MemoryInspectorWindow.h"
#include "DisassemblyWindow.h"
#include "XRefListWindow.h"
#include "LogListWindow.h"


class MainWindow : public Window {
public:
    static constexpr auto CLASS_NAME = L"MainWindowClass";

    enum MainWindowChildID : ChildID {
        ID_DUMP_CONTROL = 1001, // TODO make private
        ID_DIS_CONTROL = 1002, // TODO make private
    };

    /*
    ** Child IDs, also used in forms.
    */
    enum ChildId2 : ChildID { // TODO make private
        ID_SEGMENT_CONTROL = 1000,
        ID_XREF_CONTROL = 1003,
        ID_LOG_CONTROL = 1004,
    };
    static void RegisterClassFor(HINSTANCE hInstance, WNDPROC lpfnWndProc);

    MainWindow(Layout& lpLayout);
    MainWindow(const MainWindow& a) = delete;           // no copy constructor
    MainWindow& operator=(const MainWindow&) = delete;  // no copy assignment
    MainWindow(MainWindow&&) = delete;                  // no move constructor
    MainWindow& operator=(MainWindow&&) = delete;       // no move assignment
    virtual ~MainWindow() = default;

    virtual void CreateControl();

    std::unique_ptr<MainWindowMenu> mainWindowMenu;
    std::unique_ptr<SegmentListWindow> segmentListWindow;
    std::unique_ptr<MemoryInspectorWindow> memoryInspectorWindow;
    std::unique_ptr<DisassemblyWindow> disassemblyWindow;
    std::unique_ptr<XRefListWindow> xrefListWindow;
    std::unique_ptr<LogListWindow> logListWindow;

    void ApplyLayout();

private:
    Layout* layout;
};
