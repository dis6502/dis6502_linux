#pragma once

#include "Dialog.h"
#include "Syntax.h"
#include "Window.h"
#include "Word.h"

class MemoryInspectorControl;
class Segment;


class AssembleDialog : public Dialog {
public:
    AssembleDialog(Window& parentWindow);

    bool Show(Segment* segment, MemoryInspectorControl* memoryInspectorControl);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    Segment* segment;
    MemoryInspectorControl* memoryInspectorControl;

    LPCWSTR GetExpressionValue(LPCWSTR ptr, word* value, wstring& error);
};
