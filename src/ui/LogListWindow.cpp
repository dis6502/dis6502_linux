#include "Application.h"

#include "LogListWindow.h"
#include "StringUtility.h"


LogListWindow::LogListWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout) {}

void LogListWindow::CreateControl(ChildID childID) {
    PartWindow::CreateControl(L"ListBox",
        WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS |
        LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_DISABLENOSCROLL,
        childID);
}

void LogListWindow::AddText(wstring_view text) {
    auto index = SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)wstring(text).c_str());

    SendMessage(hWnd, LB_SETCURSEL, index, 0L);
}
