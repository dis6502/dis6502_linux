#include "Application.h"
#include "SpriteControlImpl.h"

#include "SpriteControl.h"


void SpriteControl::RegisterClassFor(HINSTANCE hInstance) {
    SpriteControlImpl::RegisterClassFor(hInstance);
}

SpriteControl::SpriteControl(Window& parentWindow, ChildID childID) :Window(parentWindow) {
    this->childID = childID;
}

void SpriteControl::CreateControl(int x, int y, int nWidth, int nHeight) {
    Window::CreateChildControl(SpriteControlImpl::CLASS_NAME,
        WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, childID, x, y, nWidth, nHeight);
}

void SpriteControl::AssignControl() {
    hWnd = GetDlgItem(parentWindow->GetHWnd(), childID);
}

void SpriteControl::SetBuffer(const ByteSequence& byteSequence) {
    SendMessage(hWnd, WM_SPRITE_SET_BUFFER, byteSequence.size(), (LPARAM)byteSequence.getConst());
}

int SpriteControl::GetModeNumberOfBytesPerLine() const {
    return (int)SendMessage(hWnd, WM_SPRITE_GET_INFO, SPRITE_NBBYTESPERLINE, 0);
}

int SpriteControl::GetNumberOfBytesPerLine() const {
    return (int)SendMessage(hWnd, WM_SPRITE_GET_NBBYTES, 0, 0);
}

WORD SpriteControl::GetMode() const {
    return (WORD)SendMessage(hWnd, WM_SPRITE_GET_MODE, 0, 0);
}

void SpriteControl::SetMode(WORD wMode, int nNumberOfBytesPerLine) {
    int wModeNumberOfBytesPerLine;

    SendMessage(hWnd, WM_SPRITE_SET_MODE, wMode, false);

    wModeNumberOfBytesPerLine = GetModeNumberOfBytesPerLine();
    if (nNumberOfBytesPerLine > wModeNumberOfBytesPerLine) {
        nNumberOfBytesPerLine = wModeNumberOfBytesPerLine;
    }

    SendMessage(hWnd, WM_SPRITE_SET_NBBYTES, nNumberOfBytesPerLine, false);
}

void SpriteControl::Refresh() const {
    SendMessage(hWnd, WM_SPRITE_REFRESH, 0, 0);
}

WORD SpriteControl::GetIndex() const {
    return (WORD)SendMessage(hWnd, WM_SPRITE_GET_INDEX, 0, 0);
}

void SpriteControl::SetIndex(WORD wIndex) {
    SendMessage(hWnd, WM_SPRITE_GET_INDEX, wIndex, false);
}

WORD SpriteControl::GetSelection() const {
    return (WORD)SendMessage(hWnd, WM_SPRITE_GET_SELECTION, 0, 0);
}

void SpriteControl::SetSelection(WORD wEnd) {
    SendMessage(hWnd, WM_SPRITE_GET_INDEX, wEnd, false);  // TODO: Word? TOO: GET_INDEX?
}
