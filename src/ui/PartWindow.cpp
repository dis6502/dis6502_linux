#include "Layout.h"
#include "PartWindow.h"
#include "Syntax.h"
#include "Window.h"
#include <Windows.h>


PartWindow::PartWindow(Window& parentWindow, PartLayout& partLayout) : Window(parentWindow), partLayout(&partLayout) {}

wstring PartWindow::GetTitle() const {
    return title;
}

void PartWindow::SetTitle(wstring_view title) {
    this->title = wstring(title);
}
void PartWindow::CreateControl(wstring_view className, DWORD dwStyle, ChildID childID) {
    Window::CreateChildControl(className, dwStyle, childID, partLayout->left, partLayout->top, partLayout->width, partLayout->height);
}

void PartWindow::ApplyLayout() {
    SetFont(partLayout->GetLayout()->GetFont());
    SetPosition(WINDOW_TOP, partLayout->left, partLayout->top, partLayout->width, partLayout->height, SWP_NOZORDER);
}
