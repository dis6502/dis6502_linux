#pragma once

#include "Syntax.h"

#include "PartWindow.h"

#include "DisassemblyControl.h"


class DisassemblyWindow : public PartWindow {
public:
	DisassemblyWindow(Window& parentWindow, PartLayout& partLayout);

	void CreateControl(INT nDisassemblyControlChildID);
	void ApplyLayout() override;

	DisassemblyControl* GetDisassemblyControl() const;

private:
	std::unique_ptr<DisassemblyControl> disassemblyControl;
};
