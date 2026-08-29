#include "Application.h"

#include "LowHighByteDialog.h"
#include "Button.h"
#include "EditControl.h"

extern std::unique_ptr<Application> g_Application;


LowHighByteDialog::LowHighByteDialog(Window& parentWindow) : Dialog(parentWindow, L"LOHIBYTEBOX") {
}

bool LowHighByteDialog::Show(MemoryType memoryType, byte knownByte) {
    if (memoryType == MemoryType::LOBYTE) {
        knownByteId = IDC_LOBYTE;
        unknownByteId = IDC_HIBYTE;
    }
    else {
        knownByteId = IDC_HIBYTE;
        unknownByteId = IDC_LOBYTE;
    }

    this->knownByte = knownByte;

    return ShowDialogBox();
}

byte LowHighByteDialog::GetUnknownByte() const {
    return unknownByte;
}

bool LowHighByteDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_INITDIALOG: {
        GetEditControl(knownByteId).SetEnabled(false);
        GetEditControl(knownByteId).SetByte(knownByte);
        GetButton(IDOK).SetEnabled(false);
        return true;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case IDOK: {
            unknownByte = GetEditControl(unknownByteId).GetByte();
            return EndDialogBox(true);
        }

        case IDCANCEL:
            return EndDialogBox( false);

        case IDC_LOBYTE:
        case IDC_HIBYTE:
            if (LOWORD(wParam) == unknownByteId)
                if (HIWORD(wParam) == EN_CHANGE) {
                    GetButton(IDOK).SetEnabled(GetEditControl(unknownByteId).HasText());
                }
            break;
            return true;
        }
        break;
    }

    return false;
}
