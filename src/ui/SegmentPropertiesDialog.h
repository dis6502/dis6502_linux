#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "ProcessorType.h"
#include <vector>

class Segment;

class SegmentPropertiesDialog: public Dialog {
public:
	SegmentPropertiesDialog(Window& parentWindow);

	bool Show(Segment& lpSegment);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

	void CreateControls() override;

private:
    std::vector<ProcessorType> processorTypes{ ProcessorType::MOS6502, ProcessorType::MOS65C02 };
	Segment* segment;
};
