#include "EquateDialog.h"
#include "Application.h"
#include "String.h"
#include "Equate.h"
#include "EquateList.h"
#include "EditControl.h"
#include "ListBox.h"
#include "Button.h"

extern std::unique_ptr<Application> g_Application;

EquateDialog::EquateDialog(Window& parentWindow) : Dialog(parentWindow, L"EQUATEBOX"),
equateList(nullptr), editable(false) {
}

bool EquateDialog::Show(EquateList& equateList, bool editable, wstring_view address) {
    this->equateList = &equateList;
    this->editable = editable;
    this->address = address;
    return ShowDialogBox();
}


bool EquateDialog::ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    int itemCount = 0;

    auto listBox = GetListBox(IDC_EQUATE_LIST);

    switch (message) {
    case WM_INITDIALOG:
        CreateControls();
        FillListbox();

        if (!address.empty()) {
            GetEditControl(IDC_EQUATE_LINE).SetText(String::Format(L"L{0} = ${1}", address, address));
            GetEditControl(IDC_EQUATE_LINE).SetFocus();
            nResult = FALSE; // Keep explicitly the just set keyboard focus
        }
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_EQUATE_LIST:
            itemCount = listBox.GetCount();
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                bool selected = false;
                for (int index = 0; index < itemCount; index++) {
                    if (listBox.IsSelectedIndex(index)) {
                        auto itemText = listBox.GetStringAtIndex(index);
                        GetEditControl(IDC_EQUATE_LINE).SetText(itemText);
                        selected = true;
                        break;
                    }
                }
                GetButton(IDC_DELETE_EQUATE).SetEnabled(selected);
            }
            return true;

        case IDC_ADD_EQUATE: {

            //const auto text = String::Trim(GetEditControl(IDC_EQUATE_LINE).GetText());
            //GetEditControl(IDC_EQUATE_LINE).SetText(text);

            //auto index = equateList->AddEquate(text);
            //if (index >= 0) {
            //    FillListbox();
            //    listBox.SetSelectedIndex(index);
            //    GetButton(IDC_DELETE_EQUATE).SetEnabled(true);
            //}
            //else {
            //    // TODO: ERROR HANDLING
            //}
            return true;
        }

        case IDC_DELETE_EQUATE:
            // Delete all selected lines.
            itemCount = listBox.GetCount();
            for (int index = itemCount - 1; index >= 0; index--) {
                if (listBox.IsSelectedIndex(index)) {
                    listBox.DeleteStringAtIndex(index);
                }
            }
            // Prevent value that was popuplated via line selection to be added by accident. 
            GetEditControl(IDC_EQUATE_LINE).Clear();
            // All selected have been deleted, sot here is no selection anymore.
            GetButton(IDC_DELETE_EQUATE).SetEnabled(false);
            return true;

        case IDC_EQUATE_LINE: {
            if (HIWORD(wParam) == EN_CHANGE) {
                const auto value = String::Trim(GetEditControl(IDC_EQUATE_LINE).GetText());
                GetButton(IDC_ADD_EQUATE).SetEnabled(!value.empty());
                return true;
            }
            break;
        }

        case IDOK: {
            SendMessage(hDlg, WM_COMMAND, IDC_ADD_EQUATE, 0L);

            equateList->Clear();
            itemCount = listBox.GetCount();
            for (int index = itemCount - 1; index >= 0; index--) {
                auto itemText = listBox.GetStringAtIndex(index);
                equateList->AddEquate(itemText);
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

VOID EquateDialog::CreateControls() {
    GetEditControl(IDC_EQUATE_LINE).SetTextSize(sizeof(EQUATE_LINE));
    GetButton(IDC_ADD_EQUATE).SetEnabled(false);
    GetButton(IDC_DELETE_EQUATE).SetEnabled(false);
}


VOID EquateDialog::FillListbox() {

    auto listBox = GetListBox(IDC_EQUATE_LIST);
    listBox.Clear();

    for (auto equate : equateList->GetEquates()) {
        listBox.AddString(equate->ToString());
    }
}
