#include "Application.h"
#include "Button.h"
#include "Control.h"
#include "Dialog.h"
#include "DiskImageExecutableFileDialog.h"
#include "EditControl.h"
#include "ListBox.h"
#include "systems/atari800/AtariDOS.h"
#include "TextLabel.h"
#include "Window.h"
#include <cstdarg>
#include <memory>
#include <Resource.h>
#include <StringUtility.h>
#include <Syntax.h>
#include <Windows.h>

extern Application* g_Application;


DiskImageExecutableFileDialog::DiskImageExecutableFileDialog(const Window& parentWindow, HFONT hComputerFont) : Dialog(parentWindow, L"OPENDISKIMAGEEXECUTABLEFILEBOX") {
    this->hComputerFont = hComputerFont;
}

INT_PTR DiskImageExecutableFileDialog::Show(wstring_view diskImageFilePath) {
    atariDisk = AtariDOS::OpenAtariDisk(diskImageFilePath);
    executableFilePath = L"";

    return Dialog::ShowDialogBox();
}

wstring DiskImageExecutableFileDialog::GetExecutableFilePath() const {
    return executableFilePath;
}

bool DiskImageExecutableFileDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    AtariFile info;
    AtariError error;
    bool bOK;

    switch (message) {
    case WM_INITDIALOG: {
        CreateControls();

        error = atariDisk->FindFirst(info);
        while (error == AtariError::OK) {
            String::Printf(String::szBuffer, L" %s %s %4hu %4hu", (info.IsLocked() ? L"*" : L" "), info.GetFileName83().c_str(), info.GetSectorCount(), info.GetStartSectorNumber());

            GetListBox(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST).AddString(String::Format(), (void*)(uintptr_t)info.GetDirectoryIndex());
            error = atariDisk->FindNext(info);
        }
        return true;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDOK: {
            auto object = GetListBox(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST).GetSelectedObject();
            if (object != nullptr) {
                const auto directoryIndex = (AtariDOS::directory_index)(uintptr_t)object;

                if (atariDisk->GetFileFromIndex(info, directoryIndex) == AtariError::OK) {
                    executableFilePath = info.GetFileName();
                    return EndDialogBox(TRUE);
                }
            }

            return true;
        }

        case IDCANCEL: {
            return EndDialogBox(FALSE);
        }

        case IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST: {
            switch (HIWORD(wParam)) {
                // the user has selected a file.
            case LBN_SELCHANGE: {
                bOK = false;
                auto index = GetListBox(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST).GetSelectedIndex();

                if (index != LB_ERR) {
                    AtariDOS::directory_index directoryIndex = SendDlgItemMessage(hDlg, IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST, LB_GETITEMDATA, (WORD)index, 0L);
                    if (atariDisk->GetFileFromIndex(info, directoryIndex) != AtariError::OK)
                        ::g_Application->SendErrorMessageWithID(IDS_ERR_ATARI_FILE);
                    else {
                        bOK = true;
                    }
                }

                GetTextLabel(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILE_NAME).SetText(bOK ? info.GetFileName() : L"");
                GetButton(IDOK).SetEnabled(bOK);
                break;
            }

                              // the user has double-clicked on a file.
            case LBN_DBLCLK: {
                auto index = GetListBox(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST).GetSelectedIndex();

                if ((index != LB_ERR) && (IsWindowEnabled(GetDlgItem(hDlg, IDOK)))) { // TODO: Have GetButton(IDOK).IsEnabled()
                    PostMessage(hDlg, WM_COMMAND, IDOK, 0);
                }
                break;
            }
            }

            return true;
        }
                                                      break;
        }
    }
    }

    return false;
}

void DiskImageExecutableFileDialog::CreateControls() {
    GetTextLabel(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST_HEADER).SetFont(hComputerFont);

    GetListBox(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILES_LIST).SetFont(hComputerFont);

    GetEditControl(IDC_DISK_IMAGE_EXECUTABLE_FILE_DISK_IMAGE_FILE_PATH).SetText(atariDisk->GetDiskImageFilePath());
    GetEditControl(IDC_DISK_IMAGE_EXECUTABLE_FILE_FILE_NAME).Clear();

    GetButton(IDOK).SetEnabled(false);
}
