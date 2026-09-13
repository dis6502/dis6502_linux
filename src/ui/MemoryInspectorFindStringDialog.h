#pragma once


#include "Dialog.h"
#include "FindStringDialog.h"
#include "Window.h"

class MemoryInspector;


class MemoryInspectorFindStringDialog : public Dialog {
public:
    MemoryInspectorFindStringDialog(Window& parentWindow);

    bool Show(MemoryInspector& memoryInspector);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;
    bool OnCancel() override;

private:
    static constexpr int MAX_CHARS = 64;
    FindStringDialog findStringDialog;

    MemoryInspector* lpMemoryInspector = nullptr;
};
