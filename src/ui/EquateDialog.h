#pragma once
#include "Syntax.h"
#include "Dialog.h"

#include "Equate.h"
class EquateList;
class ListBox;

class EquateDialog :public Dialog {
public:
	EquateDialog(Window& parentWindow);

	bool Show(EquateList& equateList, bool editable, wstring_view address);

protected:
	bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

	VOID CreateControls() override;

private:
	EquateList* equateList;

	bool editable;
	wstring address;

	VOID FillListbox();
};