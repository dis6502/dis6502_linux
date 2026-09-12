#include "Font.h"
#include "StringUtility.h"
#include "UI.h"
#include "UIApplication.h"
#include "Window.h"
#include <exception>
#include <memory>
#include <stdexcept>
#include "Syntax.h"
#include <Windows.h>

extern std::unique_ptr<UIApplication> g_UIApplication;

Window* Window::WINDOW_TOP = nullptr;

Window::Window() {
    this->parentWindow = nullptr;

    hWnd = NULL_HWND;
    lpWndProc = nullptr;

    lpWM_DROPFILESProc = nullptr;
    lpWM_MOUSEWHEELProc = nullptr;
    lpWM_RBUTTONDOWNProc = nullptr;
}

Window::Window(const Window& parentWindow) : Window() {

    this->parentWindow = &parentWindow;
}

Window::~Window() {
    DestroyWindow(hWnd);
}

HWND Window::GetHWnd() const {
    return hWnd;
}

void Window::CreateWindowControl(wstring_view className, wstring_view windowName, WindowStyle style, HMENU hMenu, int x, int y, int nWidth, int nHeight) {
    HWND hParentWnd = (parentWindow == nullptr) ? NULL_HWND : parentWindow->GetHWnd();
    HWND hWnd = CreateWindow(wstring(className).c_str(), wstring(windowName).c_str(), style, x, y, nWidth, nHeight, hParentWnd, hMenu, ::g_UIApplication->GetInstanceHandle(), nullptr);  // ##c_str() OK

    if (hWnd == NULL_HWND) {

        // If the lastError is 0, you have a bug/invalid WndProc registered for the class
        const auto lastError = GetLastError();

        wchar_t szMessage[1024];
        String::Printf(szMessage,
            L"Error %lu during CreateWindow for window '%s' of class '%s'.\n"
            L"Actual parameters are: dwStyle=%lu hParentWnd=%p hMenu=%p  ::g_Application->GetHInstance()=%p\n",
            lastError, wstring(windowName).c_str(), wstring(className).c_str(), style, hParentWnd, hMenu, ::g_UIApplication->GetInstanceHandle());
        auto message = String::wstring_to_utf8(String::Format(L"Error in CreateWindowControl: %s", szMessage));
        safeExitWithExitCode(message.c_str(), lastError);
    }

    InitControl(className, hWnd);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
    lpWndProc = (WindowProcedure)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
}

void Window::CreateChildControl(wstring_view className, WindowStyle style, ChildID childID, int x, int y, int nWidth, int nHeight) {
    CreateWindowControl(className, L"", style, (HMENU)(LONG_PTR)childID, x, y, nWidth, nHeight);
}

void Window::InitControl(wstring_view className, WindowHandle hWnd) {
    if (!this->className.empty()) {
        throw std::runtime_error("Class name already set.");
    }

    if (this->hWnd != NULL_HWND) {
        throw std::runtime_error("Window handle hWnd already set.");
    }

    this->className = className;
    this->hWnd = hWnd;
}

void Window::SetTitle(wstring_view title) {
    SetWindowText(hWnd, wstring(title).c_str());  // ##c_str() OK
}

void Window::SetFont(Font* font) {
    if (font != nullptr) {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)font->hFont, (LPARAM)false);
    }
    else {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)Font::NULL_HFONT, (LPARAM)false);
    }
}

void Window::SetPosition(Window* windowInsertAfter, int X, int Y, int cx, int cy, PositionFlags flags) {
    auto hWndInsertAfter = (windowInsertAfter == nullptr ? NULL_HWND : windowInsertAfter->GetHWnd());
    SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, flags);
}

bool Window::HasFocus() const {
    auto focusHWND = GetFocus();
    return focusHWND == hWnd;
}

void Window::SetFocus() {
    ::SetFocus(hWnd);
}

void  Window::SetDragAcceptFiles(bool bAccept) {
    DragAcceptFiles(hWnd, bAccept);
}

void Window::SetDropFilesProc(WindowProcedure lpWndProc) {
    lpWM_DROPFILESProc = lpWndProc;
}

void Window::SetMouseWheelProc(WindowProcedure lpWndProc) {
    lpWM_MOUSEWHEELProc = lpWndProc;
}

void Window::SetRButtonDownProc(WindowProcedure lpWndProc) {
    lpWM_RBUTTONDOWNProc = lpWndProc;
}

Window::LRESULT CALLBACK Window::WindowProc(WindowHandle hWnd, MESSAGE message, WPARAM wParam, LPARAM lParam) {
    Window* lpWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (lpWindow != nullptr) {
        try {
            return lpWindow->WindowProcInstance(hWnd, message, wParam, lParam);
        }
        catch (const std::exception& ex) {
            throw ex;
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::LRESULT Window::WindowProcInstance(WindowHandle hWnd, MESSAGE message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DROPFILES:
        if (lpWM_DROPFILESProc != nullptr) {
            ::SetFocus(hWnd);
            return lpWM_DROPFILESProc(hWnd, message, wParam, lParam);
        }

    case WM_MOUSEWHEEL:
        if (lpWM_MOUSEWHEELProc != nullptr) {
            return lpWM_MOUSEWHEELProc(hWnd, message, wParam, lParam);
        }

    case WM_RBUTTONDOWN:
        if (lpWM_RBUTTONDOWNProc != nullptr) {
            return lpWM_RBUTTONDOWNProc(hWnd, message, wParam, lParam);
        }
    }

    return CallWindowProc((WNDPROC)lpWndProc, hWnd, message, wParam, lParam);
}
