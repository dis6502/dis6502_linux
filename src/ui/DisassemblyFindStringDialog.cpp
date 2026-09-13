#include "Application.h"
#include "Button.h"
#include "Dialog.h"
#include "EditControl.h"
#include "Syntax.h"
#include "Text.h"
#include "DisassemblyFindStringDialog.h"


DisassemblyFindStringDialog::DisassemblyFindStringDialog(Window& parentWindow) : Dialog(parentWindow, L"DISFINDSTRINGBOX") {
}

bool DisassemblyFindStringDialog::Show(wstring_view findAscii) {
    this->findAscii = findAscii;

    return ShowDialogBox();
}

wstring DisassemblyFindStringDialog::GetFindAscii() const {
    return findAscii;
}

bool DisassemblyFindStringDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {
    case IDC_FINDASCII:
        if (HIWORD(wParam) == EN_CHANGE) {
            GetButton(IDOK).SetEnabled(GetEditControl(IDC_FINDASCII).HasText());
            return true;
        }
        break;

    case IDCANCEL:
        return OnCancel();

    case IDOK:
        return OnOK();

    default:
        return false;
    }
    return false;
}

bool DisassemblyFindStringDialog::InitDialog() {
    GetEditControl(IDC_FINDASCII).SetText(findAscii);
    GetButton(IDOK).SetEnabled(!findAscii.empty());
    return true;
}

bool DisassemblyFindStringDialog::OnOK() {
    findAscii = GetEditControl(IDC_FINDASCII).GetText();
    return EndDialogBox(true);
}
