#pragma once

#include "Byte.h"
#include "Dialog.h"
#include "MemoryType.h"
#include "Window.h"


class LowHighByteDialog : public Dialog {
public:

    LowHighByteDialog(Window& parentWindow);

    bool Show(MemoryType memoryType, byte knownByte);
    byte GetUnknownByte() const;

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    ITEM_ID knownByteId = 0;
    byte knownByte = 0;
    ITEM_ID unknownByteId = 0;
    byte unknownByte = 0;
};
