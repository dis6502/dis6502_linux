#pragma once

#include "Syntax.h"
#include "UI.h"
#include "Window.h"
#include <map>

/*
 * Macros for GetValues / SetValues
 */

#define GetItemText(ID, ATTR) GetDlgItemText(hDlg, (ID), (ATTR), sizeof( (ATTR) ));
#define SetItemTextAndSize(ID, ATTR) SendDlgItemMessageW(hDlg, (ID), EM_LIMITTEXT, sizeof( (ATTR) ) - 1, 0); SetDlgItemTextW(hDlg, (ID), (ATTR));

class Button;
class CheckBox;
class ComboBox;
class Control;
class EditControl;
class TextLabel;
class ListBox;

class Dialog : public Window {
public:

    typedef int ITEM_ID;
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
    INT_PTR ShowDialogBox();

    virtual bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) = 0;

    virtual void CreateControls();
    virtual void DeleteControls();

    // Returns true as convenience so you can use return EndDialogBox(TRUE)
    bool EndDialogBox(INT_PTR nResult);

    HWND GetItemHandle(ITEM_ID itemID) const;

private:
    static std::map<HWND, Dialog*> instances;

    wstring templateName;

    std::map<ITEM_ID, Control*> items;

    static INT_PTR CALLBACK DialogFunc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};
