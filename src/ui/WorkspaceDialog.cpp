#include "ComboBox.h"
#include "Dialog.h"
#include "Resource.h"
#include "systems/ComputerSystem.h"
#include "systems/ComputerSystemFactory.h"
#include "systems/ComputerSystemType.h"
#include "Window.h"
#include "Workspace.h"
#include "WorkspaceDialog.h"
#include <memory>
#include <Windows.h>

extern std::unique_ptr<Workspace> g_Workspace;


WorkspaceDialog::WorkspaceDialog(Window& parentWindow) : Dialog(parentWindow, L"WORKSPACEBOX") {}

bool WorkspaceDialog::Show() {
    return (ShowDialogBox() == TRUE);
}

bool WorkspaceDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {

    case WM_INITDIALOG:
        CreateControls();
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case IDOK: {
            auto object = (ComputerSystemTypeInfo*)GetComboBox(IDC_WORKSPACE_COMPUTER_SYSTEM).GetSelectedObject();
            if (object != nullptr) {
                ::g_Workspace->SetComputerSystemType(object->type);
            }
            return EndDialogBox(true);
        }

        case IDCANCEL: {
            return EndDialogBox(false);
        }

        default:
            break;
        }

    default:
        break;
    }

    return false;
}

void WorkspaceDialog::CreateControls() {
    auto comboBox = &GetComboBox(IDC_WORKSPACE_COMPUTER_SYSTEM);

    const auto computerSystemFactory = g_Workspace->GetComputerSystemFactory();
    auto info = computerSystemFactory->GetComputerSystemTypeInfo(ComputerSystemType::ATARI5200);
    comboBox->AddString(info->text, (void*)info.get());

    info = computerSystemFactory->GetComputerSystemTypeInfo(ComputerSystemType::ATARI800);
    comboBox->AddString(info->text, (void*)info.get());

    info = computerSystemFactory->GetComputerSystemTypeInfo(ComputerSystemType::C64);
    comboBox->AddString(info->text, (void*)info.get());

    info = computerSystemFactory->GetComputerSystemTypeInfo(ComputerSystemType::ORIC);
    comboBox->AddString(info->text, (void*)info.get());

    comboBox->SetSelectedString(::g_Workspace->GetComputerSystem()->GetTypeInfo()->text);
}



