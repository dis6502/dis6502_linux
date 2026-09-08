#pragma once

#include "Dialog.h"
#include "Syntax.h"

class MemoryInspectorControl;
class Segment;


class AssembleDialog : public Dialog {
public:
    AssembleDialog(Window& parentWindow);

    bool Show(Segment* segment, MemoryInspectorControl* memoryInspectorControl);

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
    Segment* segment;
    MemoryInspectorControl* memoryInspectorControl;

    LPCWSTR GetExpressionValue(LPCWSTR ptr, WORD* value, wstring& error);
};
