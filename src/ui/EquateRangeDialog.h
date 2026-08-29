#pragma once
#include "Syntax.h"
#include "Dialog.h"

#include "Equate.h"
class EquateList;

class EquateRangeDialog :public Dialog {
public:
    EquateRangeDialog(Window& parentWindow);

    bool Show(EquateList& systemEquateList, EquateList& equateList, wstring_view address);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

    void CreateControls() override;

private:

    EquateList* systemEquateList = nullptr;
    EquateList* equateList = nullptr;
    bool addressSpecified = false;
    Memory::address address;
    bool dialogVisible = false;


    typedef CHAR ITEM_LINE[128];

    void SendErrorMessage(LPCWSTR szMessage);
    void FillCombobox(const EquateList& equateList);

};