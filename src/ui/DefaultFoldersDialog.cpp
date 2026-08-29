#include <Windows.h>
#define STRICT_TYPED_ITEMIDS

#include <shobjidl.h>
#include "Text.h"
#include "systems/ComputerSystemType.h"
#include "DefaultFolders.h"
#include "FileIO.h"
#include "FolderType.h"
#include "DefaultFoldersDialog.h"
#include "EditControl.h"

DefaultFoldersDialog::DefaultFoldersDialog(Window& parentWindow) : Dialog(parentWindow, L"DEFAULTFOLDERSBOX") {
}

bool DefaultFoldersDialog::Show(HWND hWndParent, DefaultFolders& defaultFolders) {
    this->defaultFolders = &defaultFolders;

    return ShowDialogBox();
}

bool DefaultFoldersDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {
    case WM_INITDIALOG: {
        auto text = defaultFolders->GetComputerSystemTypeInfo()->text;
        auto title = Text::Format(IDS_DEFAULT_FOLDERS_DIALOG_TITLE, text);
        SetTitle(title);
        SetDialogValues(*defaultFolders);
        return true;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            GetDialogValues(*defaultFolders);
            return EndDialogBox(true);

        case IDCANCEL:
            return EndDialogBox(false);

        case IDC_BUTTON_RAW:
            nResult = SelectFolder(FolderType::RAW_FILES, IDC_PATH_RAW);
            return true;

        case IDC_BUTTON_BIN:
            nResult = SelectFolder(FolderType::EXECUTABLE_FILES, IDC_PATH_BIN);
            return true;

        case IDC_BUTTON_ROM:
            nResult = SelectFolder(FolderType::ROM_IMAGE_FILES, IDC_PATH_ROM);
            return true;

        case IDC_BUTTON_CAS:
            nResult = SelectFolder(FolderType::CASSETTE_IMAGE_FILES, IDC_PATH_CAS);
            return true;

        case IDC_BUTTON_DSK:
            nResult = SelectFolder(FolderType::DISK_IMAGE_FILES, IDC_PATH_DSK);
            return true;

        case IDC_BUTTON_WRK:
            nResult = SelectFolder(FolderType::WORKSPACE_FILES, IDC_PATH_WRK);
            return true;

        case IDC_BUTTON_EQU:
            nResult = SelectFolder(FolderType::EQUATES_FILES, IDC_PATH_EQU);
            return true;

        case IDC_BUTTON_PRF:
            nResult = SelectFolder(FolderType::PROFILE_FILES, IDC_PATH_PRF);
            return true;

        case IDC_BUTTON_ASM:
            nResult = SelectFolder(FolderType::DISASSEMBLY_FILES, IDC_PATH_ASM);
            return true;

        default:
            return false;

        }

    default:
        return false;
    }
}

void DefaultFoldersDialog::SetDialogValues(const DefaultFolders& defaultFolders) {
    FileIO::FOLDER_PATH szFolderPath;

    defaultFolders.CopyFolderPath(FolderType::RAW_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_RAW, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::EXECUTABLE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_BIN, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::CASSETTE_IMAGE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_CAS, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::ROM_IMAGE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_ROM, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::DISK_IMAGE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_DSK, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::WORKSPACE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_WRK, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::EQUATES_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_EQU, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::PROFILE_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_PRF, szFolderPath);

    defaultFolders.CopyFolderPath(FolderType::DISASSEMBLY_FILES, szFolderPath);
    SetItemTextAndSize(IDC_PATH_ASM, szFolderPath);
}

void DefaultFoldersDialog::GetDialogValues(DefaultFolders& defaultFolders) {

    defaultFolders.SetFolderPath(FolderType::RAW_FILES, GetEditControl(IDC_PATH_RAW).GetText());
    defaultFolders.SetFolderPath(FolderType::EXECUTABLE_FILES, GetEditControl(IDC_PATH_BIN).GetText());
    defaultFolders.SetFolderPath(FolderType::CASSETTE_IMAGE_FILES, GetEditControl(IDC_PATH_CAS).GetText());
    defaultFolders.SetFolderPath(FolderType::ROM_IMAGE_FILES, GetEditControl(IDC_PATH_ROM).GetText());
    defaultFolders.SetFolderPath(FolderType::DISK_IMAGE_FILES, GetEditControl(IDC_PATH_DSK).GetText());
    defaultFolders.SetFolderPath(FolderType::WORKSPACE_FILES, GetEditControl(IDC_PATH_WRK).GetText());
    defaultFolders.SetFolderPath(FolderType::EQUATES_FILES, GetEditControl(IDC_PATH_EQU).GetText());
    defaultFolders.SetFolderPath(FolderType::PROFILE_FILES, GetEditControl(IDC_PATH_PRF).GetText());
    defaultFolders.SetFolderPath(FolderType::DISASSEMBLY_FILES, GetEditControl(IDC_PATH_ASM).GetText());
}



HRESULT ShellSelectFolder(HWND parentHWnd, wstring_view title, wstring& folderPath)
{
    // CoCreate the File Open Dialog object.
    IFileDialog* fileDialog = NULL;
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fileDialog));
    if (SUCCEEDED(hr))
    {
        // Set the options on the dialog.
        DWORD dwFlags;

        // Before setting, always get the options first in order 
        // not to override existing options.
        hr = fileDialog->GetOptions(&dwFlags);
        if (SUCCEEDED(hr))
        {
            // In this case, get shell items only for file system items.
            hr = fileDialog->SetOptions(dwFlags | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
            if (SUCCEEDED(hr))
            {
                hr = fileDialog->SetTitle(wstring(title).c_str());
                if (SUCCEEDED(hr))
                {
                    // Show the dialog
                    hr = fileDialog->Show(parentHWnd);
                    if (SUCCEEDED(hr))
                    {
                        // Obtain the result once the user clicks 
                        // the 'Open' button.
                        // The result is an IShellItem object.
                        IShellItem* psiResult;
                        hr = fileDialog->GetResult(&psiResult);
                        if (SUCCEEDED(hr))
                        {
                            // We are just going to print out the 
                            // name of the file for sample sake.
                            PWSTR pszFilePath = NULL;
                            hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                &pszFilePath);
                            if (SUCCEEDED(hr)) {
                                folderPath = wstring(pszFilePath);
                                CoTaskMemFree(pszFilePath);
                            }
                            psiResult->Release();
                        }
                    }
                }
            }
        }
        fileDialog->Release();
    }
    return (hr == S_OK ? true : false);
}

bool DefaultFoldersDialog::SelectFolder(const FolderType folderType, Dialog::ITEM_ID itemID) {

    auto title = Text::Format(IDS_DEFAULT_FOLDERS_DIALOG_SUB_TITLE, (wstring_view)FolderTypeFactory::GetInfo(folderType).text);

    auto currentFolderPath = GetEditControl(itemID).GetText();

    if (ShellSelectFolder(this->GetHWnd(), title, currentFolderPath)) {
        GetEditControl(itemID).SetText(currentFolderPath);
    }

    return false;
}