#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "MemoryType.h"


class LowHighByteDialog : public Dialog {
public:

	LowHighByteDialog(Window& parentWindow);

	bool Show(MemoryType memoryType, byte knownByte);
	byte GetUnknownByte() const;

protected:
	bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
	ITEM_ID knownByteId = 0;
	byte knownByte = 0;
	ITEM_ID unknownByteId = 0;
	byte unknownByte = 0;
};
