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

bool WorkspaceDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {

    case IDOK:
        return OnOK();

    case IDCANCEL:
        return OnCancel();

    default:
        break;
    }
    return false;
}

bool WorkspaceDialog::InitDialog() {
    CreateControls();
    return true;
}

bool WorkspaceDialog::OnOK() {
    auto object = (ComputerSystemTypeInfo*)GetComboBox(IDC_WORKSPACE_COMPUTER_SYSTEM).GetSelectedObject();
    if (object != nullptr) {
        ::g_Workspace->SetComputerSystemType(object->type);
    }
    return EndDialogBox(true);
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



