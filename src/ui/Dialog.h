#pragma once

#include "Syntax.h"
#include "Window.h"
#include <map>
#include <memory>
#include <Windows.h>

/*
 * Macros for GetValues / SetValues
 */

#define GetItemText(ID, ATTR) GetDlgItemText(hDlg, (ID), (ATTR), sizeof( (ATTR) ));
#define SetItemTextAndSize(ID, ATTR) SendDlgItemMessageW(hDlg, (ID), EM_LIMITTEXT, sizeof( (ATTR) ) - 1, 0); SetDlgItemTextW(hDlg, (ID), (ATTR));

#include "Button.h"
class CheckBox;
class ComboBox;
class Control;
class EditControl;
class TextLabel;
class ListBox;

class Dialog : public Window {
public:

    using ITEM_ID = int;
    using DialogFuncResult = INT_PTR;

    ~Dialog() override;

    Control& GetItem(ITEM_ID itemID);
    Button& GetButton(ITEM_ID itemID);
    CheckBox& GetCheckBox(ITEM_ID itemID);
    ComboBox& GetComboBox(ITEM_ID itemID);
    EditControl& GetEditControl(ITEM_ID itemID);
    TextLabel& GetTextLabel(ITEM_ID itemID);
    ListBox& GetListBox(ITEM_ID itemID);

protected:
    HWND hDlg;

    Dialog(const Window& parentDialog, wstring_view templateName);
    DialogFuncResult ShowDialogBox();

    // ProcessDialogMessage return true, if the message was process.
    // If false is returned, the default window procedure will be called.
    // Default implementation only dispatches WM_COMMAND to ProcessCommand; override for other messages.
    virtual bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult);

    virtual bool InitDialog(); // Return true for default keyboard focus.
    virtual bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam);
    virtual bool OnOK();
    virtual bool OnCancel();

    virtual void CreateControls();
    virtual void DeleteControls();

    // Returns true as convenience so you can use return EndDialogBox(TRUE)
    bool EndDialogBox(DialogFuncResult nResult);

    HWND GetItemHandle(ITEM_ID itemID) const;

    // Shows a modal alert. Guards messageDialogVisible while the alert's nested message loop runs.
    void SendErrorMessage(wstring_view title, wstring_view message);


private:
    static std::map<HWND, Dialog*> instances;

    wstring templateName;

    std::map<ITEM_ID, std::unique_ptr<Control>> items;

    static DialogFuncResult CALLBACK DialogFunc(HWND hDlg, MESSAGE message, WPARAM wParam, LPARAM lParam);

    // Set while SendErrorMessage's nested message loop runs, so overrides of ProcessDialogMessage
    // can forward re-entrant messages to DefWindowProc instead of processing them again.
    bool messageDialogVisible = false;
};
