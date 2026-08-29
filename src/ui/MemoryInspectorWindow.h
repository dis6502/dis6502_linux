#pragma once

#include "Syntax.h"

#include "PartWindow.h"
#include "Memory.h"
#include "MemoryInspectorControl.h"

class PartLayout;
class MemoryInspectorSelection;
class SpriteControl;


class MemoryInspectorWindow : public PartWindow {
public:
	MemoryInspectorWindow(Window& parentWindow, PartLayout& partLayout);

	void CreateControl(int nMemoryInspectorControlChildId);
	void ApplyLayout() override;


	MemoryInspectorControl* GetMemoryInspectorControl() const;

	void ClearSelection();
	void SetSelection(Memory::offset nBegin, Memory::offset nEnd);

private:
	std::unique_ptr<MemoryInspectorControl> memoryInspectorControl;
	MemoryInspectorSelection* memoryInspectorSelection;
};
