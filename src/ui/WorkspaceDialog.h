#pragma once

#include "Syntax.h"

#include "Dialog.h"

class Workspace;

class WorkspaceDialog : public Dialog {
public:
	WorkspaceDialog(Window& parentWindow);

	bool Show();

protected:
	bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

	void CreateControls() override;

};
