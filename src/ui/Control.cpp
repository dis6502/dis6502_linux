#include "Control.h"
#include "UI.h"
#include "Window.h"
#include <map>
#include <stdexcept>
#include <utility>
#include <Windows.h>

static std::map<HWND, Control*> instances;


Control* Control::GetInstance(HWND hWnd) {
    Control* lpResult;
    try {
        lpResult = instances.at(hWnd);
    }
    catch (const std::out_of_range&) {
        lpResult = new Control(hWnd);
    }
    return lpResult;
}

Control::Control(HWND hWnd) {
    this->hWnd = hWnd;
    instances.insert(std::pair<HWND, Control*>(this->hWnd, this));
}

Control::~Control() {
    instances.erase(hWnd);
}

void Control::SetEnabled(bool enabled) {
    EnableWindow(hWnd, enabled ? TRUE : FALSE);
}

void Control::SetFont(const HFONT& hFont) {
    SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, (LPARAM)FALSE);
}

bool Control::HasFocus() const {
    return GetFocus() == hWnd;
}

void Control::SetFocus() {
    ::SetFocus(hWnd);
}

void Control::SetScrollRange(int bar, ScrollPosition minPosition, ScrollPosition maxPosition, BOOL update) {
    ::SetScrollRange(hWnd, bar, minPosition, maxPosition, update);
}

void Control::SetScrollPosition(int bar, ScrollPosition scrollPosition, BOOL update) {
    SetScrollPos(hWnd, bar, scrollPosition, TRUE);
}
LRESULT Control::WndProc(Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    // Empty default implementation
    return 0;
}

LRESULT CALLBACK Control::WndProc(HWND hWnd, Window::MESSAGE message, WPARAM wParam, LPARAM lParam) {
    Control* lpControl = GetInstance(hWnd);
    const LRESULT result = lpControl->WndProc(message, wParam, lParam);

    if (message == WM_DESTROY) {
        delete lpControl;
    }
    return result;
}