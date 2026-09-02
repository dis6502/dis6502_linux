#include "Application.h"

#include "Button.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "Control.h"
#include "Debug.h"
#include "Dialog.h"
#include "EditControl.h"
#include "ListBox.h"
#include "MessageBoxDialog.h"
#include "StringUtility.h"
#include "TextLabel.h"

extern std::unique_ptr<Application> g_Application;

std::map<HWND, Dialog*> Dialog::instances;


Dialog::Dialog(const Window& parentWindow, wstring_view templateName) : Window(parentWindow), hDlg(NULL_HWND) {
    this->templateName = templateName;
}

Dialog::~Dialog() {
    if (hDlg != NULL_HWND) {
        instances.erase(hDlg);
    }
}

INT_PTR Dialog::ShowDialogBox() {
    auto hInstance = ::g_Application->GetInstanceHandle();

    const auto result = DialogBoxParamW(hInstance, templateName.c_str(), parentWindow->GetHWnd(), DialogFunc, (LPARAM)this);
    if (result == (INT_PTR)(-1)) {

        // If it fails with error code 0, there is a problem with creation of the control for a "CONTROL" entry in the .rc file
        const auto error = GetLastError();
        throw std::runtime_error(String::wstring_to_utf8(String::Format(L"Creation dialog with template {0} failed with error code {1}", templateName, std::to_hex_string((uintmax_t)error))));
    }
    return result;
}

void Dialog::CreateControls() {}

void Dialog::DeleteControls() {
    for (auto item : items) { // OK: Creates copy of item itself
        delete item.second;
    }
}

bool Dialog::EndDialogBox(INT_PTR nResult) {
    DeleteControls();
    const auto result = EndDialog(hDlg, nResult);

    if (!result) {
        throw std::runtime_error("Cannot destroy modal dialog box");
    }
    return true;
}

/**
 * https://learn.microsoft.com/en-us/windows/win32/api/winuser/nc-winuser-dlgproc
 * Remarks
   You should use the dialog box procedure only if you use the dialog box class for the dialog box.
   This is the default class and is used when no explicit class is specified in the dialog box template.
   Although the dialog box procedure is similar to a window procedure, it must not call the DefWindowProc
   function to process unwanted messages. Unwanted messages are processed internally by the dialog box
   window procedure.
**/
INT_PTR CALLBACK Dialog::DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    Dialog* dialog = nullptr;
    // https://wiki.winehq.org/List_Of_Windows_Messages
    // http://blog.airesoft.co.uk/2009/11/wm_messages/
    //Debug::Log(String::Format(L"hDlg={0} message={1} wParam={2} lParam={3}",
    //    std::to_hex_wstring((uintmax_t)hDlg), std::to_hex_wstring((uintmax_t)message), std::to_hex_wstring((uintmax_t)wParam), std::to_hex_wstring((uintmax_t)lParam)));

    switch (message) {

    case WM_INITDIALOG:
        dialog = (Dialog*)lParam;

        instances.insert(std::pair<HWND, Dialog*>(hDlg, dialog));

        if (dialog->GetHWnd() == NULL_HWND) {
            dialog->InitControl(dialog->templateName, hDlg);
        }

        dialog->hDlg = hDlg;
        break; // This means the WM_INITDIALOG message will also be passed to the dialog instance

    default:
        if (!instances.contains(hDlg)) {
            return NULL_INT_PTR; // Do not call DefWindowProc(hDlg, message, wParam, lParam);
        }
        dialog = instances.at(hDlg);
        break;
    }

    try {
        INT_PTR  nResult = 0;
        if (message == WM_INITDIALOG) {
            nResult = TRUE; // Default for automatic keyboard focus
        }
        if (dialog->ProcessDialogMessage(message, wParam, lParam, nResult)) {
            Debug::Log(String::Format(L"hDlg={0} message={1} wParam={2} lParam={3}",
                std::to_hex_string((uintmax_t)hDlg), std::to_hex_string((uintmax_t)message), std::to_hex_string((uintmax_t)wParam), std::to_hex_string((uintmax_t)lParam)));
            Debug::LogValue(L"nResult", std::to_hex_string((uintmax_t)nResult));
            return nResult;
        };
        return NULL_INT_PTR; // Do not call DefWindowProc(hDlg, message, wParam, lParam);

    }
    catch (const std::exception& ex) {
        MessageBoxDialog::ShowAlert(nullptr, String::Format(L"Exception in Dialog {0}", dialog->templateName), String::utf8_to_wstring(ex.what()));
        throw ex;
    }
}

HWND Dialog::GetItemHandle(ITEM_ID itemID) const {
    const auto hWnd = GetDlgItem(hDlg, itemID);
    if (hWnd == NULL_HWND) {
        throw std::runtime_error(String::wstring_to_utf8(String::Format(L"Invalid item id {0}", std::to_wstring(itemID))));
    }
    return hWnd;
}

Control& Dialog::GetItem(ITEM_ID itemID) {
    auto it = items.find(itemID);
    Control* result = nullptr;
    if (it == items.end()) {
        result = new Control(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = it->second;
    }
    return *result;
}

Button& Dialog::GetButton(ITEM_ID itemID) {
    auto it = items.find(itemID);
    Button* result = nullptr;
    if (it == items.end()) {
        result = new Button(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<Button*>(it->second);
    }
    return *result;
}

CheckBox& Dialog::GetCheckBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    CheckBox* result = nullptr;
    if (it == items.end()) {
        result = new CheckBox(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<CheckBox*>(it->second);
    }
    return *result;
}

ComboBox& Dialog::GetComboBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    ComboBox* result = nullptr;
    if (it == items.end()) {
        result = new ComboBox(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<ComboBox*>(it->second);
    }
    return *result;
}

EditControl& Dialog::GetEditControl(ITEM_ID itemID) {
    auto it = items.find(itemID);
    EditControl* result = nullptr;
    if (it == items.end()) {
        result = new EditControl(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<EditControl*>(it->second);
    }
    return *result;
}

TextLabel& Dialog::GetTextLabel(ITEM_ID itemID) {
    auto it = items.find(itemID);
    TextLabel* result = nullptr;
    if (it == items.end()) {
        result = new TextLabel(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<TextLabel*>(it->second);
    }
    return *result;
}


ListBox& Dialog::GetListBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    ListBox* result = nullptr;
    if (it == items.end()) {
        result = new ListBox(GetItemHandle(itemID));
        items.insert(std::pair(itemID, result));
    }
    else {
        result = static_cast<ListBox*>(it->second);
    }
    return *result;
}

