#pragma once

#include "Dialog.h"
#include "Syntax.h"
#include "Window.h"
#include <Windows.h>

class Disassembly;


class DisassemblyFindStringDialog : public Dialog {
public:
    DisassemblyFindStringDialog(Window& parentWindow);

    bool Show(wstring_view findAscii);
    wstring GetFindAscii() const;

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
    wstring findAscii;
};
