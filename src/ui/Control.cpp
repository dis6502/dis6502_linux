#include "Control.h"
#include "Font.h"
#include "Window.h"
#include <map>
#include <stdexcept>
#include <utility>
#include <Windows.h>

static std::map<Control::WindowHandle, Control*> instances;


Control* Control::GetInstance(WindowHandle hWnd) {
    Control* lpResult;
    try {
        lpResult = instances.at(hWnd);
    }
    catch (const std::out_of_range&) {
        lpResult = new Control(hWnd);
    }
    return lpResult;
}

Control::Control(WindowHandle hWnd) {
    this->hWnd = hWnd;
    instances.insert(std::pair<WindowHandle, Control*>(this->hWnd, this));
}

Control::~Control() {
    instances.erase(hWnd);
}

void Control::SetEnabled(bool enabled) {
    EnableWindow(hWnd, enabled ? TRUE : FALSE);
}

void Control::SetFont(Font* font) {
    if (font != nullptr) {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)font->hFont, (LPARAM)FALSE);
    }
    else {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)Font::NULL_HFONT, (LPARAM)FALSE);

    }
}

bool Control::HasFocus() const {
    return GetFocus() == hWnd;
}

void Control::SetFocus() {
    ::SetFocus(hWnd);
}

void Control::SetScrollRange(int bar, ScrollPosition minPosition, ScrollPosition maxPosition, bool update) {
    ::SetScrollRange(hWnd, bar, minPosition, maxPosition, update);
}

void Control::SetScrollPosition(int bar, ScrollPosition scrollPosition, bool update) {
    SetScrollPos(hWnd, bar, scrollPosition, TRUE);
}

Control::LRESULT Control::WndProc(MESSAGE message, WPARAM wParam, LPARAM lParam) {
    // Empty default implementation
    return 0;
}

Control::LRESULT Control::WndProc(WindowHandle hWnd, MESSAGE message, WPARAM wParam, LPARAM lParam) {
    Control* lpControl = GetInstance(hWnd);
    const LRESULT result = lpControl->WndProc(message, wParam, lParam);

    if (message == WM_DESTROY) {
        delete lpControl;
    }
    return result;
}