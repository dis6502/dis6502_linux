#include "Application.h"
#include "DisassemblyControlImpl.h"

#include "DisassemblyControl.h"
#include "DisassemblyResult.h"
#include "Strings.h"

void DisassemblyControl::RegisterClassFor(HINSTANCE hInstance) {
    DisassemblyControlImpl::RegisterClassFor(hInstance);
}

DisassemblyControl::DisassemblyControl(const Window& parentWindow, ChildID childID) : Window(parentWindow) {
    this->childID = childID;
}

void DisassemblyControl::CreateControl(int x, int y, int nWidth, int nHeight) {
    Window::CreateChildControl(DisassemblyControlImpl::CLASS_NAME,
        WS_VSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS,
        childID, x, y, nWidth, nHeight);
}

bool DisassemblyControl::AreLineNumbersActive() const {
    return (bool)SendMessage(hWnd, WM_DIS_GET_LINE_NUMBERS_ACTIVE, WPARAM_0, LPARAM_0);
}

void DisassemblyControl::SetLineNumbersActive(bool lineNumbersActive) {
    SendMessage(hWnd, WM_DIS_SET_LINE_NUMBERS_ACTIVE, (BOOL)lineNumbersActive, LPARAM_0);
}

void DisassemblyControl::SetResult(DisassemblyResult& result) {
    SendMessage(hWnd, WM_DIS_SET_RESULT, WPARAM_0, (LPARAM)&result);
}

void DisassemblyControl::Refresh() {
    SendMessage(hWnd, WM_DIS_REFRESH, WPARAM_0, LPARAM_0);
}

void DisassemblyControl::SelectLine(LineNumber lineNumber) {
    SendMessage(hWnd, WM_DIS_SELECT_LINE, WPARAM_0, (LPARAM)lineNumber);
}

bool DisassemblyControl::SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {
    const auto lParam = MAKELONG(offset, segmentNumber);
    const bool bFound = SendMessage(hWnd, WM_DIS_SELECT_LINE_WITH_INFO, WPARAM_0, lParam);

    return bFound;
}

bool DisassemblyControl::ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset) {
    const auto lParam = MAKELONG(offset, segmentNumber);
    const bool bFound = SendMessage(hWnd, WM_DIS_EXTEND_SEL, WPARAM_0, lParam);

    return bFound;
}

void DisassemblyControl::SetScrollLineNumber(LineNumber lineNumber) {
    SendMessage(hWnd, WM_DIS_SET_SCROLL_LINE_NUMBER, LPARAM_0, lineNumber);
}

void DisassemblyControl::SyncLine(LineNumber lineNumber) {
    SendMessage(hWnd, WM_DIS_SYNC_LINE, 0, lineNumber);
}

size_t DisassemblyControl::GetHistoryIndex() const {
    return (size_t)SendMessage(hWnd, WM_DIS_GET_HISTORY_INDEX, WPARAM_0, LPARAM_0);
}

void DisassemblyControl::ResetHistory() {
    SendMessage(hWnd, WM_DIS_RESET_HISTORY, 0, 0);
}

void DisassemblyControl::BackInHistory() {
    SendMessage(hWnd, WM_DIS_BACK_IN_HISTORY, 0, 0);
}

wstring DisassemblyControl::GetLabelReference() const {
    return String::FromCString((LPCWSTR)SendMessage(hWnd, WM_DIS_GET_LABEL_REFERENCE, 0, LPARAM_0));
}

void DisassemblyControl::SelectLabelDefinition(wstring_view label) {
    SendMessage(hWnd, WM_DIS_SELECT_DEFINITION, WPARAM_0, (LPARAM)(wstring(label).c_str()));
}

wstring DisassemblyControl::GetLabelDefinition() const {
    return String::FromCString((LPCWSTR)SendMessage(hWnd, WM_DIS_GET_LABEL_DEFINITION, 0, LPARAM_0));
}

void DisassemblyControl::SelectAllLabelReferences(wstring_view label) {
    SendMessage(hWnd, WM_DIS_SELECT_ALL_REFERENCES, WPARAM_0, (LPARAM)(wstring(label).c_str()));
}
