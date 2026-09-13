#include "AboutDialog.h"
#include "Application.h"
#include "ByteArray.h"
#include "Dialog.h"
#include "FileIO.h"
#include "ListBox.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "Text.h"
#include <memory>
#include <span> 

extern Application* g_Application;

AboutDialog::Module::Module(wstring_view name) : name(name), version(L""), description(L"") {}

AboutDialog::AboutDialog(Window& parentWindow) : Dialog(parentWindow, L"ABOUTBOX") {
    modules.push_back(std::make_unique<Module>(L"DIS6502.exe"));
}

void AboutDialog::Show() {
    ShowDialogBox();
}

bool AboutDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {
    case WM_COMMAND:
        return ProcessCommand((COMMAND)LOWORD(wParam), wParam, lParam);

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG: {
        // Change background color to white.
        SetBkColor((HDC)wParam, RGB(255, 255, 255));
        nResult = (INT_PTR)GetStockObject(WHITE_BRUSH);
        return true;
    }

    default:
        break;
    }

    return false;

}

bool AboutDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    if (command == IDOK || command == IDCANCEL) {
        return EndDialogBox(true);
    }
    return false;
}

bool AboutDialog::InitDialog() {
    CreateControls();
    GetAllModuleVersions();
    return true;
}

/*
** Get version info for one filename.
*/
void AboutDialog::GetModuleVersion(Module& module) {

    // Build full filename.
    auto filePath = ::g_Application->GetModuleFilePath(module.name);

    // Get info structure.
    module.version = L"-";
    module.description = L"-";

    if (FileIO::FileExists(filePath)) {

        DWORD handle = 0;
        const auto fileVersionInfoSize = GetFileVersionInfoSizeW(filePath.c_str(), &handle);  // #.c_str() OK
        if (fileVersionInfoSize != 0) {
            ByteArray versionData(fileVersionInfoSize);
            handle = 0;
            GetFileVersionInfo(filePath.c_str(), handle, fileVersionInfoSize, versionData.get()); // #.c_str() OK

            // Get module version.
            WORD* lpBuffer = nullptr;
            UINT size = 0;
            if (VerQueryValue(versionData.get(), L"\\VarFileInfo\\Translation", (LPVOID*)&lpBuffer, &size)) {

                if (lpBuffer != nullptr && size != 0) {
                    wchar_t szName[256] = {};
                    auto lpName = szName;
                    const std::span<WORD> wordSpan{ lpBuffer,2 };
                    const auto version1 = wordSpan[0];
                    const auto version2 = wordSpan[1];
                    wchar_t* lpValue = nullptr;

                    String::Printf(lpName, L"\\StringFileInfo\\%04hX%04hX\\ProductVersion", version1, version2);
                    if (VerQueryValue(versionData.get(), lpName, (LPVOID*)&lpValue, &size)) {
                        module.version = wstring(lpValue);
                    }

                    String::Printf(lpName, L"\\StringFileInfo\\%04hX%04hX\\FileDescription", version1, version2);
                    if (VerQueryValue(versionData.get(), lpName, (LPVOID*)&lpValue, &size)) {
                        module.description = wstring(lpValue);
                    }
                }
            }
        }
    }
}

/*
** Get version info for all modules.
*/
void AboutDialog::GetAllModuleVersions() {

    for (const auto& module : modules) {
        GetModuleVersion(*module);
        GetListBox(IDC_LIST_VERSION).AddString(String::Format(L"{0}\t{1}\t{2}", module->name, module->version, module->description));
    }
}
