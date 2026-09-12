#include "Application.h"
#include "EquateDialog.h"
#include "EquateList.h"
#include "EquateListController.h"
#include "EquateListLogic.h"
#include "EquateRangeDialog.h"
#include "FileDialogs.h"
#include "FileType.h"
#include "MessageBoxDialog.h"
#include "Syntax.h"
#include "WorkspaceTypes.h"
#include "systems/ComputerSystem.h"
#include "Text.h"
#include "Workspace.h"
#include <memory>


extern std::unique_ptr<FileDialogs> g_FileDialogs;

EquateListController::EquateListController(EquateListLogic& equateListLogic) : parentWindow(nullptr) {
    this->equateListLogic = &equateListLogic;
}

void EquateListController::SetParentWindow(Window& window) {
    this->parentWindow = &window;
}

void EquateListController::LoadUserEquates(Workspace& workspace) {
    /*
    ** Ask the user for a filename.
    */
    auto result = g_FileDialogs->ChooseOpenFileName(*parentWindow, lastEquateFilePath, FileType::EQUATES_FILE);
    if (result.success) {
        lastEquateFilePath = result.filePath;
        equateListLogic->Load(*workspace.GetUserEquateList(), result.filePath);
    }
}

void EquateListController::Save(const EquateList& equateList, bool xasm) const {

    /*
    ** Ask the user for a filename.
    */
    auto result = g_FileDialogs->ChooseSaveFileName(*parentWindow, lastEquateFilePath, FileType::EQUATES_FILE);
    if (result.success) {
        equateListLogic->Save(equateList, result.filePath, xasm);
    }
}

void EquateListController::Clear(EquateList& equateList) {
    if (!equateList.IsEmpty()) {

        Text::TextID textID = IDS_EQUATES_CONFIRM_CLEAR_SYSTEM_EQUATES;
        if (equateList.GetProperty() == WorkspaceProperty::USER_EQUATES) {
            textID = IDS_EQUATES_CONFIRM_CLEAR_USER_EQUATES;
        }
        if (MessageBoxDialog::ShowQuestion(parentWindow, L"Clear Equates", Text::Get(textID), MessageBoxDialog::ButtonType::YESNO) == MessageBoxDialog::ButtonID::YES) {
            equateList.Clear();
        }
    }
}

void EquateListController::Edit(EquateList& equateList, bool editable, wstring_view address) {
    EquateDialog dialog(*parentWindow);
    dialog.Show(equateList, editable, address);
}

void EquateListController::DefineUserAddressRange(Workspace& workspace, wstring_view address) {
    EquateRangeDialog dialog(*parentWindow);
    dialog.Show(*workspace.GetSystemEquateList(), *workspace.GetUserEquateList(), address);
}
