#include "Application.h"
#include "Button.h"
#include "Byte.h"
#include "Dialog.h"
#include "EditControl.h"
#include "MemoryType.h"
#include "Text.h"
#include "LowHighByteDialog.h"

extern Application* g_Application;


LowHighByteDialog::LowHighByteDialog(Window& parentWindow) : Dialog(parentWindow, L"LOHIBYTEBOX") {}

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

bool LowHighByteDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {

    case IDOK:
        return OnOK();

    case IDCANCEL:
        return OnCancel();

    case IDC_LOBYTE:
    case IDC_HIBYTE:
        if (command == unknownByteId)
            if (HIWORD(wParam) == EN_CHANGE) {
                GetButton(IDOK).SetEnabled(GetEditControl(unknownByteId).HasText());
            }
        break;
    }

    return false;
}

bool LowHighByteDialog::InitDialog() {
    GetEditControl(knownByteId).SetEnabled(false);
    GetEditControl(knownByteId).SetByte(knownByte);
    GetButton(IDOK).SetEnabled(false);
    return true;
}

bool LowHighByteDialog::OnOK() {
    unknownByte = GetEditControl(unknownByteId).GetByte();
    return EndDialogBox(true);
}
