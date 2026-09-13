#pragma once
#include "Dialog.h"
#include "Memory.h"
#include "Syntax.h"

#include "Equate.h"
#include "Window.h"
class EquateList;

class EquateRangeDialog :public Dialog {
public:
    EquateRangeDialog(Window& parentWindow);

    bool Show(EquateList& systemEquateList, EquateList& equateList, wstring_view address);

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

    void CreateControls() override;

private:

    EquateList* systemEquateList = nullptr;
    EquateList* equateList = nullptr;
    bool addressSpecified = false;
    Memory::address address;


    typedef CHAR ITEM_LINE[128];

    void FillCombobox(const EquateList& equateList);

};