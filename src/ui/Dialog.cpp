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
#include "Syntax.h"
#include "TextLabel.h"
#include "UI.h"
#include "UIApplication.h"
#include "Window.h"
#include <cstdint>
#include <exception>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <Windows.h>

extern std::unique_ptr<UIApplication> g_UIApplication;

std::map<HWND, Dialog*> Dialog::instances;


Dialog::Dialog(const Window& parentWindow, wstring_view templateName) : Window(parentWindow), hDlg(NULL_HWND) {
    this->templateName = templateName;
}

Dialog::~Dialog() {
    if (hDlg != NULL_HWND) {
        instances.erase(hDlg);
    }
}

Dialog::DialogFuncResult Dialog::ShowDialogBox() {
    auto hInstance = ::g_UIApplication->GetInstanceHandle();

    const auto result = DialogBoxParamW(hInstance, templateName.c_str(), parentWindow->GetHWnd(), DialogFunc, (LPARAM)this);
    if (result == (INT_PTR)(-1)) {

        // If it fails with error code 0, there is a problem with creation of the control for a "CONTROL" entry in the .rc file
        const auto error = GetLastError();
        throw std::runtime_error(String::wstring_to_utf8(String::Format(L"Creation dialog with template {0} failed with error code {1}", templateName, std::to_hex_string((uintmax_t)error))));
    }
    return result;
}

bool Dialog::InitDialog() {
    return true;
}

bool Dialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    switch (message) {
    case WM_COMMAND:
        return ProcessCommand((COMMAND)LOWORD(wParam), wParam, lParam);

    default:
        return false;
    }
}

bool Dialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    return false;
}

bool Dialog::OnOK() {
    return EndDialogBox(true);
}

bool Dialog::OnCancel() {
    return EndDialogBox(false);
}

void Dialog::SendErrorMessage(wstring_view title, wstring_view message) {
    messageDialogVisible = true;
    MessageBoxDialog::ShowAlert(this, title, message);
    messageDialogVisible = false;
}

void Dialog::CreateControls() {}

void Dialog::DeleteControls() {
    items.clear();
}

bool Dialog::EndDialogBox(DialogFuncResult nResult) {
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
Dialog::DialogFuncResult CALLBACK Dialog::DialogFunc(HWND hDlg, MESSAGE message, WPARAM wParam, LPARAM lParam) {
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
        return dialog->InitDialog();

    default:
        if (!instances.contains(hDlg)) {
            return NULL_INT_PTR; // Do not call DefWindowProc(hDlg, message, wParam, lParam);
        }
        dialog = instances.at(hDlg);
    }

    // If there is another message dialog popup, process its messages separately.
    if (dialog->messageDialogVisible) {
        return DefWindowProc(hDlg, message, wParam, lParam);
    }

    try {
        DialogFuncResult  nResult = 0;
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
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<Control>(GetItemHandle(itemID))).first;
    }
    return *it->second;
}

Button& Dialog::GetButton(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<Button>(GetItemHandle(itemID))).first;
    }
    return static_cast<Button&>(*it->second);
}

CheckBox& Dialog::GetCheckBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<CheckBox>(GetItemHandle(itemID))).first;
    }
    return static_cast<CheckBox&>(*it->second);
}

ComboBox& Dialog::GetComboBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<ComboBox>(GetItemHandle(itemID))).first;
    }
    return static_cast<ComboBox&>(*it->second);
}

EditControl& Dialog::GetEditControl(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<EditControl>(GetItemHandle(itemID))).first;
    }
    return static_cast<EditControl&>(*it->second);
}

TextLabel& Dialog::GetTextLabel(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<TextLabel>(GetItemHandle(itemID))).first;
    }
    return static_cast<TextLabel&>(*it->second);
}


ListBox& Dialog::GetListBox(ITEM_ID itemID) {
    auto it = items.find(itemID);
    if (it == items.end()) {
        it = items.emplace(itemID, std::make_unique<ListBox>(GetItemHandle(itemID))).first;
    }
    return static_cast<ListBox&>(*it->second);
}

