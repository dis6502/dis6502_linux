#include "EquateRangeDialog.h"
#include "Application.h"
#include "DatatypeUtility.h"
#include "String.h"
#include "EquateList.h"
#include "MessageBoxDialog.h"
#include "ComboBox.h"
#include "ListBox.h"
#include "EditControl.h"
#include "Button.h"

constexpr auto ITEM_FORMAT = L"%s ($%04hX)";

EquateRangeDialog::EquateRangeDialog(Window& parentWindow) :Dialog(parentWindow, L"EQUATERANGEBOX") {
}

bool EquateRangeDialog::Show(EquateList& systemEquateList, EquateList& equateList, wstring_view address) {
    this->systemEquateList = &systemEquateList;
    this->equateList = &equateList;
    this->addressSpecified = DatatypeUtility::WordFromString(this->address, wstring(L"0x").append(address));
    this->dialogVisible = false;
    return ShowDialogBox();
}


bool EquateRangeDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    if (dialogVisible) {
        return DefWindowProc(hDlg, message, wParam, lParam);
    }

    switch (message) {
    case WM_INITDIALOG: {
        CreateControls();
        return true;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {

            const auto wStartAddr = GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).GetAddress();
            const auto wEndAddr = GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).GetAddress();
            const Equate* selectedEquate = (Equate*)GetComboBox(IDC_EQUATE_RANGE_BASE_EQUATE).GetSelectedObject();

            if (wStartAddr == 0) {
                SendErrorMessage(L"Invalid start address."); // TODO translatable text
            }
            else if (wEndAddr == 0) {
                SendErrorMessage(L"Invalid end address."); // TODO translatable text
            }
            else if (wStartAddr > wEndAddr) {
                SendErrorMessage(L"Start address greater than end address."); // TODO translatable text
            }
            else if (selectedEquate == nullptr) {
                SendErrorMessage(L"No base equate selected."); // TODO translatable text
            }
            else {
                const auto wAddr = selectedEquate->GetLabelValue();
                if ((wAddr >= wStartAddr) && (wAddr <= wEndAddr)) {
                    SendErrorMessage(L"Equate address is inside range."); // TODO translatable text
                }
                else {
                    equateList->SetRange(selectedEquate->GetLabel(), wAddr, wStartAddr, wEndAddr);
                    return EndDialogBox(true);
                }
            }
            return true;

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

VOID EquateRangeDialog::CreateControls() {

    GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).Clear();
    GetEditControl(IDC_EQUATE_RANGE_START_ADDRESS).SetTextSize(4);
    GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).Clear();
    GetEditControl(IDC_EQUATE_RANGE_END_ADDRESS).SetTextSize(4);
    FillCombobox(*systemEquateList);
    FillCombobox(*equateList);

}


void EquateRangeDialog::SendErrorMessage(LPCWSTR szMessage) {
    dialogVisible = true;
    MessageBoxDialog::Show(this, L"Define address range", szMessage, MB_OK | MB_ICONEXCLAMATION);
    dialogVisible = false;
}

void EquateRangeDialog::FillCombobox(const EquateList& equateList) {
    const auto baseEquateComboBox = &GetComboBox(IDC_EQUATE_RANGE_BASE_EQUATE);

    for (auto& equate : equateList.GetEquates()) {
        if (!equate->IsRange()) {
            wsprintf(String::szBuffer, ITEM_FORMAT, equate->GetLabel(), equate->GetLabelValue());
            baseEquateComboBox->AddString(String::Format(), equate);

            if (this->addressSpecified && equate->GetLabelValue() == this->address) {
                baseEquateComboBox->SetSelectedIndex(baseEquateComboBox->GetCount() - 1);
            }
        }
    }
}
