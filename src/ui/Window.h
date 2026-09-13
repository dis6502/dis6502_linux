#pragma once

#include "Syntax.h"
#include <Windows.h>
class Menu;

class AbstractWindow {
public:
    using WindowHandle = HWND;
    using MESSAGE = unsigned int;
    using WPARAM = unsigned __int64;
    using LPARAM = __int64;
    using LRESULT = __int64;
    using COMMAND = unsigned short;
    using INT_PTR = __int64;
    using ChildID = int;
    using WindowStyle = unsigned long;
    using PositionFlags = unsigned int;

    using WindowProcedure = LRESULT(__stdcall*)(WindowHandle, MESSAGE, WPARAM, LPARAM);


    static constexpr WPARAM WPARAM_0 = 0;
    static constexpr LPARAM LPARAM_0 = 0;
    static constexpr LRESULT LRESULT_0 = 0;
};

class Font;

class Window :public AbstractWindow {
public:

    static Window* WINDOW_TOP;

    WindowHandle GetHWnd() const;

    void SetTitle(wstring_view title);
    void SetFont(Font* font);

    void SetPosition(Window* windowInsertAfter, int X, int Y, int cx, int cy, PositionFlags flags);

    bool HasFocus() const;
    void SetFocus();

    void SetMouseWheelProc(WindowProcedure lpWndProc);
    void SetRButtonDownProc(WindowProcedure lpWndProc);

    void SetDragAcceptFiles(bool bAccept);
    void SetDropFilesProc(WindowProcedure lpWndProc);

protected:
    const Window* parentWindow;
    WindowHandle hWnd;

    Window();
    Window(const Window& parentWindow);
    virtual ~Window();

    void CreateWindowControl(wstring_view className, wstring_view windowName, WindowStyle style, Menu* menu, int x, int y, int nWidth, int nHeight);
    void CreateChildControl(wstring_view className, WindowStyle style, ChildID childID, int X, int Y, int nWidth, int nHeight);
    void InitControl(wstring_view className, WindowHandle hWnd);

private:
    wstring className;
    WindowProcedure lpWndProc;

    WindowProcedure lpWM_DROPFILESProc;
    WindowProcedure lpWM_MOUSEWHEELProc;
    WindowProcedure lpWM_RBUTTONDOWNProc;

    // Win32 callback requires __stdcall
    static LRESULT __stdcall WindowProc(WindowHandle hWND, MESSAGE message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT WindowProcInstance(WindowHandle hWnd, MESSAGE message, WPARAM wParam, LPARAM lParam);
};
