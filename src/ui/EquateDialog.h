#pragma once
#include "Syntax.h"
#include "Dialog.h"

#include "Equate.h"
#include "Window.h"
class EquateList;
class ListBox;

class EquateDialog :public Dialog {
public:
	EquateDialog(Window& parentWindow);

	bool Show(EquateList& equateList, bool editable, wstring_view address);

protected:
	bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
	bool InitDialog() override;
	bool OnOK() override;

	VOID CreateControls() override;

private:
	EquateList* equateList;

	bool editable;
	wstring address;

	VOID FillListbox();
};