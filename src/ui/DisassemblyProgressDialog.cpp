#include "Dialog.h"
#include "Disassembly.h"
#include "DisassemblyProgressDialog.h"
#include "Resource.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "TextLabel.h"
#include "UI.h"
#include "Window.h"
#include <string>


DisassemblyProgressDialog::DisassemblyProgressDialog(Window& parentWindow) : Dialog(parentWindow, L"DISPROCESSBOX") {
    Init(nullptr);
}

void DisassemblyProgressDialog::Init(Disassembly* disassembly) {
    this->disassembly = disassembly;
    cancelled = false;
    updateCount = 0;
}

void DisassemblyProgressDialog::DisassembleInternal(Disassembly& disassembly) {
    Init(&disassembly);

    ShowDialogBox();
}

void DisassemblyProgressDialog::SetPass(wstring_view pass) {
    this->pass = pass;
    GetTextLabel(IDC_PASS_NUMBER).SetText(pass);
    GetTextLabel(IDC_SEGMENT_NUMBER).SetText(L"");
}

void DisassemblyProgressDialog::SetSegmentNumber(SEGMENT_NUMBER segmentNumber) {
    GetTextLabel(IDC_SEGMENT_NUMBER).SetText(std::to_wstring(segmentNumber));
}

bool DisassemblyProgressDialog::IsCancelled() {
    MSG msg;

    // Peek every 16 calls if there are pending messages, so the user can actually click "Cancel".
    if ((updateCount++ & 0x0F) == 0) {

        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return cancelled;
}

/*
** Call disassembly process while displaying a dialog box.
** The user can click on CANCEL to abort the process.
*/
bool DisassemblyProgressDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_USER_COMMAND:
        disassembly->DisassembleInternal();
        return EndDialogBox(true);

    case WM_COMMAND:
        return ProcessCommand((COMMAND)LOWORD(wParam), wParam, lParam);
    default:
        break;
    }

    return false;
}

bool DisassemblyProgressDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    if (command == IDCANCEL) {
        return OnCancel();
    }
    return false;
}

bool DisassemblyProgressDialog::InitDialog() {
    RedrawScreen(hDlg);
    PostMessage(hDlg, WM_USER_COMMAND, 0, 0L);
    return true;
}

bool DisassemblyProgressDialog::OnCancel() {
    cancelled = true;
    return true;
}

void DisassemblyProgressDialog::RedrawScreen(HWND hDlg) {
    MSG msg;

    SetCapture(hDlg);

    while (PeekMessage(&msg, NULL_HWND, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ReleaseCapture();
}
