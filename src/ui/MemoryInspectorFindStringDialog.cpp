#include "Button.h"
#include "CheckBox.h"
#include "Dialog.h"
#include "FindStringDialog.h"
#include "MemoryInspector.h"
#include "MemoryInspectorFindStringDialog.h"
#include "Syntax.h"
#include "Text.h"



MemoryInspectorFindStringDialog::MemoryInspectorFindStringDialog(Window& parentWindow) :
    Dialog(parentWindow, L"DUMPFINDSTRINGBOX"), findStringDialog(FindStringDialog(MAX_CHARS)) {}

bool MemoryInspectorFindStringDialog::Show(MemoryInspector& memoryInspector) {
    this->lpMemoryInspector = &memoryInspector;

    return ShowDialogBox();
}

bool MemoryInspectorFindStringDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    bool find = false;

    switch (command) {
    case IDC_FINDHEX:
        if ((HIWORD(wParam) == EN_CHANGE)) {
            find = findStringDialog.HexStringToAsciiString(*this);
            GetButton(IDOK).SetEnabled(find);
        }
        break;

    case IDC_FINDASCII:
        if ((HIWORD(wParam) == EN_CHANGE)) {
            find = findStringDialog.AsciiStringToHexString(*this);
            GetButton(IDOK).SetEnabled(find);
        }
        break;

    case IDCANCEL:
        return OnCancel();

    case IDOK:
        return OnOK();
    }

    return false;
}

bool MemoryInspectorFindStringDialog::InitDialog() {
    wstring findString;
    bool allSegments = false;

    lpMemoryInspector->GetFindParameters(findString, allSegments);

    // TODO Remember String/hex selection as byte array instead, store hex to support 00 also
    findStringDialog.SetAsciiString(*this, findString);

    const auto find = findStringDialog.AsciiStringToHexString(*this);

    CheckRadioButton(hDlg, IDC_RADIO_ALL, IDC_RADIO_SELECTED, (allSegments ? IDC_RADIO_ALL : IDC_RADIO_SELECTED));
    GetButton(IDOK).SetEnabled(find);

    return true;
}

bool MemoryInspectorFindStringDialog::OnOK() {
    const auto find = findStringDialog.HexStringToAsciiString(*this);
    if (find) {
        const auto allSegments = GetCheckBox(IDC_RADIO_ALL).IsChecked();

        const auto found = lpMemoryInspector->FindString(findStringDialog.GetAsciiString(), allSegments);
        if (found) {
            EndDialog(hDlg, true);
        }

        return true;
    }

    return false;
}

bool MemoryInspectorFindStringDialog::OnCancel() {
    EndDialog(hDlg, false);
    return true;
}
