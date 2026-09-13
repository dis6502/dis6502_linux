#include "Application.h"
#include "ComboBox.h"
#include "DatatypeUtility.h"
#include "Dialog.h"
#include "EditControl.h"
#include "Equate.h"
#include "EquateList.h"
#include "EquateRangeDialog.h"
#include "EquateRangeDialog.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "Text.h"

constexpr auto ITEM_FORMAT = L"%s ($%04hX)";

EquateRangeDialog::EquateRangeDialog(Window& parentWindow) :Dialog(parentWindow, L"EQUATERANGEBOX") {}

bool EquateRangeDialog::Show(EquateList& systemEquateList, EquateList& equateList, wstring_view address) {
    this->systemEquateList = &systemEquateList;
    this->equateList = &equateList;
    this->addressSpecified = DatatypeUtility::WordFromString(this->address, wstring(L"0x").append(address));
    return ShowDialogBox();
}


bool EquateRangeDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_COMMAND:
        return ProcessCommand((COMMAND)LOWORD(wParam), wParam, lParam);

    default:
        break;

    }
    return false;
}

bool EquateRangeDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
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

bool EquateRangeDialog::InitDialog() {
    CreateControls();
    return true;
}

bool EquateRangeDialog::OnOK() {
    const auto wStartAddr = GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).GetAddress();
    const auto wEndAddr = GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).GetAddress();
    const Equate* selectedEquate = (Equate*)GetComboBox(IDC_EQUATE_RANGE_BASE_EQUATE).GetSelectedObject();

    auto title = L"Define address range";

    if (wStartAddr == 0) {
        SendErrorMessage(title, L"Invalid start address."); // TODO translatable text
    }
    else if (wEndAddr == 0) {
        SendErrorMessage(title, L"Invalid end address."); // TODO translatable text
    }
    else if (wStartAddr > wEndAddr) {
        SendErrorMessage(title, L"Start address greater than end address."); // TODO translatable text
    }
    else if (selectedEquate == nullptr) {
        SendErrorMessage(title, L"No base equate selected."); // TODO translatable text
    }
    else {
        const auto wAddr = selectedEquate->GetLabelValue();
        if ((wAddr >= wStartAddr) && (wAddr <= wEndAddr)) {
            SendErrorMessage(title, L"Equate address is inside range."); // TODO translatable text
        }
        else {
            equateList->SetRange(selectedEquate->GetLabel(), wAddr, wStartAddr, wEndAddr);
            return EndDialogBox(true);
        }
    }
    return true;
}

VOID EquateRangeDialog::CreateControls() {

    GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).Clear();
    GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).SetTextSize(4);
    GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).Clear();
    GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).SetTextSize(4);
    FillCombobox(*systemEquateList);
    FillCombobox(*equateList);

}


void EquateRangeDialog::FillCombobox(const EquateList& equateList) {
    const auto baseEquateComboBox = &GetComboBox(IDC_EQUATE_RANGE_BASE_EQUATE);

    for (auto& equate : equateList.GetEquates()) {
        if (!equate->IsRange()) {
            baseEquateComboBox->AddString(String::Printf(ITEM_FORMAT, equate->GetLabel(), equate->GetLabelValue()), equate);

            if (this->addressSpecified && equate->GetLabelValue() == this->address) {
                baseEquateComboBox->SetSelectedIndex(baseEquateComboBox->GetCount() - 1);
            }
        }
    }
}
