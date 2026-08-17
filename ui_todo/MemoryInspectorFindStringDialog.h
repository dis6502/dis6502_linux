#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "FindStringDialog.h"

class MemoryInspector;


class MemoryInspectorFindStringDialog : public Dialog {
public:
	MemoryInspectorFindStringDialog(Window& parentWindow);

	bool Show(MemoryInspector& memoryInspector);

protected:
	bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	static constexpr int MAX_CHARS = 64;
	FindStringDialog findStringDialog;

	MemoryInspector* lpMemoryInspector = nullptr;
	bool recursion = false;
};
