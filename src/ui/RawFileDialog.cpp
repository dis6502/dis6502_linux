#include "Application.h"
#include "Button.h"
#include "ByteArray.h"
#include "CommonIO.h"
#include "Control.h"
#include "Dialog.h"
#include "EditControl.h"
#include "FileIO.h"
#include "Memory.h"
#include "MemoryInspectorControl.h"
#include "RawFileDialog.h"
#include "Resource.h"
#include "Syntax.h"
#include "Window.h"
#include <memory>
#include <Windows.h>

extern Application* g_Application;


RawFileDialog::RawFileDialog(const Window& parentWindow, HFONT hComputerFont) : Dialog(parentWindow, L"OPENRAWFILEBOX") {
    this->hComputerFont = hComputerFont;
    Clear();

}

RawFileDialog::~RawFileDialog() {
    Clear();
}

void RawFileDialog::Clear() {
    filePath = L"";
    fileBuffer = ByteArray(size_t{ 0 });

    nBegin = 0;
    nSize = 0;
    wAddr = 0;
}

INT_PTR RawFileDialog::Show(wstring_view filePath) {
    this->filePath = filePath;
    Clear();


    try {
        fileBuffer = FileIO::ReadByteArray(filePath);
    }
    catch (const IOException& ex) {
        g_Application->SendErrorMessageWithException(ex);
    }

    if (fileBuffer.empty()) {
        return false; // TODO: Message File is mepty
    }

    return ShowDialogBox();
}

ByteArray RawFileDialog::GetFileBuffer() const {
    return fileBuffer;
}

void RawFileDialog::GetResult(Memory::offset& nBegin, Memory::size& nSize, Memory::address& wAddr) const {
    nBegin = this->nBegin;
    nSize = this->nSize;
    wAddr = this->wAddr;
}

bool RawFileDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_INITDIALOG:
        CreateControls();
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case IDOK: {
            wAddr = GetEditControl(IDC_RAW_FILE_ADDRESS).GetAddress();
            Memory::offset nEnd;
            memoryInspectorControl->GetSelection(nBegin, nEnd, true);
            nSize = nEnd - nBegin + 1;

            return EndDialogBox(true);
        }

        case IDCANCEL: {
            return EndDialogBox(false);
        }

        case IDC_RAW_FILE_ADDRESS: {
            if (HIWORD(wParam) == EN_CHANGE) {
                // TODO use/create EditoControl->IsValidAddress
                GetButton(IDOK).SetEnabled(GetEditControl(IDC_RAW_FILE_ADDRESS).HasText());
            }
            return true;
        }

        default:
            break;

        }
        break;

    default:
        break;
    }

    return false;
}

void RawFileDialog::CreateControls() {
    GetEditControl(IDC_RAW_FILE_PATH).SetText(filePath);

    memoryInspectorControl = std::make_unique<MemoryInspectorControl>(*this, IDC_RAW_FILE_DUMP);
    memoryInspectorControl->BindControl();
    memoryInspectorControl->SetFont(hComputerFont);
    memoryInspectorControl->SetNumberOfBytesPerLine(16);
    memoryInspectorControl->SetBuffer(fileBuffer.get(), fileBuffer.size());
    memoryInspectorControl->ClearSelection();

    GetEditControl(IDC_RAW_FILE_ADDRESS).Clear();

    GetButton(IDOK).SetEnabled(false);
}

void RawFileDialog::DeleteControls() {
    memoryInspectorControl.reset();
}
