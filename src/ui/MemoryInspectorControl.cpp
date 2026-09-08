

#include "MemoryInspectorControl.h"
#include "MemoryInspectorControlImpl.h"
#include "MemoryInspectorControlTypes.h"


void MemoryInspectorControl::RegisterClassFor(HINSTANCE hInstance) {
    MemoryInspectorControlImpl::RegisterClassFor(hInstance);
}

MemoryInspectorControl::MemoryInspectorControl(const Window& parentWindow, ChildID childID) :Window(parentWindow) {
    this->childID = childID;
}

void MemoryInspectorControl::CreateControl(int x, int y, int nWidth, int nHeight) {
    Window::CreateChildControl(MemoryInspectorControlImpl::CLASS_NAME,
        WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
        childID, x, y, nWidth, nHeight);

    SetNumberOfBytesPerLine(8);
}

void MemoryInspectorControl::BindControl() {
    HWND hParentWnd = parentWindow->GetHWnd();
    HWND hWnd = GetDlgItem(hParentWnd, childID);
    Window::InitControl(MemoryInspectorControlImpl::CLASS_NAME, hWnd);

    // Take over current positioning.
    RECT Rect;
    GetWindowRect(hWnd, &Rect);

    MapWindowPoints(HWND_DESKTOP, hParentWnd, (LPPOINT)&Rect, 2);
    SetPosition(HWND_TOP, Rect.left, Rect.top, Rect.right - Rect.left + 1, Rect.bottom - Rect.top + 1, SWP_NOZORDER);
}

void MemoryInspectorControl::SetNumberOfBytesPerLine(int nNumberOfBytesPerLine) {
    SendMessage(hWnd, WM_DUMP_SET_NUMBER_OF_BYTES_PER_LINE, WPARAM_0, (LPARAM)nNumberOfBytesPerLine);
}

void MemoryInspectorControl::SetInternal(bool bInternal) {
    SendMessage(hWnd, WM_DUMP_SET_INTERNAL, bInternal, true);
}

void MemoryInspectorControl::SetBuffer(byte* lpBuffer, unsigned int nBufferSize) {
    this->nBufferSize = nBufferSize;
    SendMessage(hWnd, WM_DUMP_SET_BUFFER, nBufferSize, (LPARAM)lpBuffer);
}

void MemoryInspectorControl::SetTypeBuffer(MemoryType* lpTypeBuffer, Memory::address startAddress) {
    SendMessage(hWnd, WM_DUMP_SET_TYPE_BUFFER, startAddress, (LPARAM)lpTypeBuffer);
}

void MemoryInspectorControl::Refresh() {
    SendMessage(hWnd, WM_DUMP_REFRESH, WPARAM_0, LPARAM_0);
}

void MemoryInspectorControl::SetScroll(UINT nLine) {
    SendMessage(hWnd, WM_DUMP_SET_SCROLL, nLine, false);
}

void MemoryInspectorControl::ClearSelection() {
    SendMessage(hWnd, WM_DUMP_SET_BEGIN_SELECTION, DUMP_NO_SELECTION, false);
    SendMessage(hWnd, WM_DUMP_SET_END_SELECTION, DUMP_NO_SELECTION, false);
}

bool MemoryInspectorControl::GetRawSelection(Memory::offset& wBegin, Memory::offset& wEnd) const {
    wBegin = SendMessage(hWnd, WM_DUMP_GET_BEGIN_SELECTION, WPARAM_0, LPARAM_0);
    wEnd = SendMessage(hWnd, WM_DUMP_GET_END_SELECTION, WPARAM_0, LPARAM_0);

    if (wBegin == DUMP_NO_SELECTION) {
        return false;
    }

    return true;
}

bool MemoryInspectorControl::GetSelection(Memory::offset& wBegin, Memory::offset& wEnd, bool bDefaultAll) const {
    const bool result = GetRawSelection(wBegin, wEnd);
    if (result) {
        if (wBegin > wEnd) {
            auto wTemp = wBegin;
            wBegin = wEnd;
            wEnd = wTemp;
        }

        if (wEnd > nBufferSize - 1) {
            wEnd = nBufferSize - 1;
        }
    }
    else {
        if (bDefaultAll) {
            wBegin = 0;
            wEnd = nBufferSize - 1;
        }
    }

    return result;
}

void MemoryInspectorControl::GetNonEmptySelection(Memory::offset& wBegin, Memory::offset& wEnd) const {
    const bool result = GetSelection(wBegin, wEnd);
    if (!result) {
        throw std::runtime_error("No selection");
    }
}

void MemoryInspectorControl::SetSelection(Memory::offset wBegin, Memory::offset wEnd) {
    SendMessage(hWnd, WM_DUMP_SET_BEGIN_SELECTION, wBegin, false);
    SendMessage(hWnd, WM_DUMP_SET_END_SELECTION, wEnd, false);
    SendMessage(hWnd, WM_DUMP_SEEK_SELECTION, WPARAM_0, LPARAM_0);
    SendMessage(hWnd, WM_DUMP_REFRESH, WPARAM_0, LPARAM_0);
}

bool MemoryInspectorControl::IsEditMode() const {
    return SendMessage(hWnd, WM_DUMP_GET_EDIT_MODE, WPARAM_0, LPARAM_0);
}

void MemoryInspectorControl::SetEditMode(bool bEditMode) {
    SendMessage(hWnd, WM_DUMP_SET_EDIT_MODE, bEditMode, true);
}
