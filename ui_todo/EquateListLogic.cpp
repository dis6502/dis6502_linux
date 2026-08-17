#include "Application.h"
#include "EquateDialog.h"
#include "EquateList.h"
#include "EquateListLogic.h"
#include "EquateRangeDialog.h"
#include "FileSystemLogic.h"
#include "FileType.h"
#include "MessageBoxDialog.h"
#include "systems/ComputerSystem.h"
#include "Text.h"
#include "Workspace.h"


extern std::unique_ptr<FileSystemLogic> g_FileSystemLogic;

EquateListLogic::EquateListLogic() : parentWindow(nullptr) {

}

void EquateListLogic::SetParentWindow(Window& window) {
    this->parentWindow = &window;
}

void EquateListLogic::LoadSystemEquates(Workspace& workspace) {
    const auto& computerSystem = workspace.GetComputerSystem();
    auto filePath = computerSystem->GetResourceFilePathByExtension(L".equ");
    workspace.GetSystemEquateList()->Load(filePath);
}

void EquateListLogic::LoadUserEquates(Workspace& workspace) {
    /*
    ** Ask the user for a filename.
    */
    auto result = g_FileSystemLogic->GetOpenFileName(*parentWindow, lastEquateFilePath, FileType::EQUATES_FILE);
    if (result.success) {
        lastEquateFilePath = result.filePath;
        workspace.GetUserEquateList()->Load(result.filePath);
    }
}

void EquateListLogic::Save(const EquateList& equateList, bool xasm) const {

    /*
    ** Ask the user for a filename.
    */
    auto result = g_FileSystemLogic->GetSaveFileName(*parentWindow, lastEquateFilePath, FileType::EQUATES_FILE);
    if (result.success) {
        equateList.Save(result.filePath, xasm);
    }
}

void EquateListLogic::Clear(EquateList& equateList) {
    if (!equateList.IsEmpty()) {

        Text::TextID textID = IDS_EQUATES_CONFIRM_CLEAR_SYSTEM_EQUATES;
        if (equateList.GetProperty() == WorkspaceProperty::USER_EQUATES) {
            textID = IDS_EQUATES_CONFIRM_CLEAR_USER_EQUATES;
        }
        if (MessageBoxDialog::Show(parentWindow, L"Clear Equates", Text::Get(textID), MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
            equateList.Clear();
        }
    }
}

void EquateListLogic::Edit(EquateList& equateList, bool editable, wstring_view address) {
    EquateDialog dialog(*parentWindow);
    dialog.Show(equateList, editable, address);
}

void EquateListLogic::DefineUserAddressRange(Workspace& workspace, wstring_view address) {
    EquateRangeDialog dialog(*parentWindow);
    dialog.Show(*workspace.GetSystemEquateList(), *workspace.GetUserEquateList(), address);
}
