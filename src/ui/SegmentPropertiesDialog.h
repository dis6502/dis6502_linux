#pragma once


#include "Dialog.h"
#include "ProcessorType.h"
#include "Window.h"
#include <vector>

class Segment;

class SegmentPropertiesDialog: public Dialog {
public:
	SegmentPropertiesDialog(Window& parentWindow);

	bool Show(Segment& lpSegment);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

	void CreateControls() override;

private:
    std::vector<ProcessorType> processorTypes{ ProcessorType::MOS6502, ProcessorType::MOS65C02 };
	Segment* segment;
};
