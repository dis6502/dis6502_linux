#include "Application.h"
#include "PartWindow.h"
#include "Syntax.h"

#include "SegmentListWindow.h"


SegmentListWindow::SegmentListWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout) {
}

void SegmentListWindow::CreateControl(ChildID childID) {
	PartWindow::CreateControl(L"ListBox",
		WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS |
		LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_DISABLENOSCROLL,
		childID);
}

void SegmentListWindow::ClearList() {
	SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
}

void SegmentListWindow::AddSegment(wstring_view text) {
	SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)wstring(text).c_str());
}

int SegmentListWindow::GetSelectedIndex() const {
	return SendMessage(hWnd, LB_GETCURSEL, 0, 0);
}

void SegmentListWindow::SetSelectedIndex(int nIndex) {
	SendMessage(hWnd, LB_SETCURSEL, nIndex, 0L);
}
