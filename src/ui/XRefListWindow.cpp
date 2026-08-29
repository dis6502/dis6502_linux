#include "Application.h"

#include "XRefListWindow.h"


XRefListWindow::XRefListWindow(Window& parentWindow, PartLayout& partLayout) : PartWindow(parentWindow, partLayout) {
}

void XRefListWindow::CreateControl(ChildID childID) {
	PartWindow::CreateControl(L"ListBox",
		WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS |
		LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_DISABLENOSCROLL,
		childID);
}
