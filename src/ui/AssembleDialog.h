#pragma once

#include "Dialog.h"
#include "Window.h"

class MemoryInspectorControl;
class Workspace;
class Segment;


class AssembleDialog : public Dialog {
public:
    AssembleDialog(Window& parentWindow);

    bool Show(Workspace& workspace, Segment& segment, MemoryInspectorControl& memoryInspectorControl);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    Workspace* workspace;
    Segment* segment;
    MemoryInspectorControl* memoryInspectorControl;
};
