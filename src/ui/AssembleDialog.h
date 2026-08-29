#pragma once

#include "Syntax.h"
#include "UI.h"

#include "Dialog.h"
#include "Segment.h"

class MemoryInspectorControl;
class Segment;


class AssembleDialog : public Dialog {
public:
	AssembleDialog(Window& parentWindow);

	bool Show(Segment* segment, MemoryInspectorControl* memoryInspectorControl);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	Segment* segment;
	MemoryInspectorControl* memoryInspectorControl;

	LPCWSTR GetExpressionValue(LPCWSTR ptr, WORD *value, wstring& error);
};
