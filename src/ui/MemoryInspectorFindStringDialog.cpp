#include "Application.h"
#include "MemoryInspector.h"

#include "MemoryInspectorFindStringDialog.h"
#include "CheckBox.h"
#include "Button.h"

extern std::unique_ptr<Application> g_Application;


MemoryInspectorFindStringDialog::MemoryInspectorFindStringDialog(Window& parentWindow) :
	Dialog(parentWindow, L"DUMPFINDSTRINGBOX"), findStringDialog(FindStringDialog(MAX_CHARS)) {
}

bool MemoryInspectorFindStringDialog::Show(MemoryInspector& memoryInspector) {
	this->lpMemoryInspector = &memoryInspector;
	this->recursion = false;

	return ShowDialogBox();
}

bool MemoryInspectorFindStringDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

	// Recursion triggered by event?
	if (recursion) {
		return false;
	}

	recursion = true;
	bool result = false;
	bool find = false;

	switch (message) {
	case WM_INITDIALOG: {
		wstring findString;
		bool allSegments = false;

		lpMemoryInspector->GetFindParameters(findString, allSegments);

		// TODO Remember String/hex selection as byte array instead, store hex to support 00 also
		findStringDialog.SetAsciiString(*this, findString);

		find = findStringDialog.AsciiStringToHexString(*this);

		CheckRadioButton(hDlg, IDC_RADIO_ALL, IDC_RADIO_SELECTED, (allSegments ? IDC_RADIO_ALL : IDC_RADIO_SELECTED));
		GetButton(IDOK).SetEnabled(find);

		result = true;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
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
			EndDialog(hDlg, false);
			result = true;
			break;

		case IDOK:
			find = findStringDialog.HexStringToAsciiString(*this);
			if (find) {

				const auto allSegments = GetCheckBox(IDC_RADIO_ALL).IsChecked();

				const auto found = lpMemoryInspector->FindString(findStringDialog.GetAsciiString(), allSegments);
				if (found) {
					EndDialog(hDlg, true);
				}

				result = true;
			}
		}
		break;
	}

	recursion = false;

	return result;
}
