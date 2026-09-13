#pragma once

#include "Dialog.h"
#include "Syntax.h"
#include "Window.h"

class Disassembly;


class DisassemblyFindStringDialog : public Dialog {
public:
    DisassemblyFindStringDialog(Window& parentWindow);

    bool Show(wstring_view findAscii);
    wstring GetFindAscii() const;

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    wstring findAscii;
};
