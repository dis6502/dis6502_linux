#pragma once

#include "Syntax.h"

#include "Dialog.h"

class Disassembly;


class DisassemblyFindStringDialog : public Dialog {
public:
	DisassemblyFindStringDialog(Window& parentWindow);

	bool Show(wstring_view findAscii);
	wstring GetFindAscii() const;

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	wstring findAscii;
};
