#pragma once


#include "Dialog.h"
#include "Window.h"

class Workspace;

class WorkspaceDialog : public Dialog {
public:
	WorkspaceDialog(Window& parentWindow);

	bool Show();

protected:
	bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
	bool InitDialog() override;
	bool OnOK() override;

	void CreateControls() override;

};
