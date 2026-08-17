#include "Application.h"
#include "Button.h"
#include "EditControl.h"
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

bool DisassemblyFindStringDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {
    case WM_INITDIALOG:
        GetEditControl(IDC_FINDASCII).SetText(findAscii);
        GetButton(IDOK).SetEnabled(!findAscii.empty());
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_FINDASCII:
            if (HIWORD(wParam) == EN_CHANGE) {
                GetButton(IDOK).SetEnabled(GetEditControl(IDC_FINDASCII).HasText());
                return true;
            }
            break;

        case IDCANCEL:
            return EndDialogBox(false);

        case IDOK:
            findAscii = GetEditControl(IDC_FINDASCII).GetText();
            return EndDialogBox(true);

        default:
            return false;
        }
    default:
        return false;
    }

}
