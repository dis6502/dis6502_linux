#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "SegmentTypes.h"
#include "Byte.h"
#include "ByteArray.h"
#include <gsl/gsl>

class SegmentWriteBootDiskDialog: public Dialog {
public:
	SegmentWriteBootDiskDialog(const Window& parentWindow);

	void Show(gsl::not_null<const Segment*> segment, bool withInitAddress, Memory::address initAddress);

protected:
	bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
    const Segment* segment;
	bool withInitAddress;
	Memory::address initAddress;
    ByteArray sector;


	void WriteBootDisk(HWND hDlg, wstring_view filePath);
};
