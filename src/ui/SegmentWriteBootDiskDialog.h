#pragma once

#include "ByteArray.h"
#include "Dialog.h"
#include "Memory.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "Window.h"
#include <gsl/gsl>

class SegmentWriteBootDiskDialog : public Dialog {
public:
    SegmentWriteBootDiskDialog(const Window& parentWindow);

    void Show(gsl::not_null<const Segment*> segment, bool withInitAddress, Memory::address initAddress);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

private:
    const Segment* segment;
    bool withInitAddress;
    Memory::address initAddress;
    ByteArray sector;


    void WriteBootDisk(HWND hDlg, wstring_view filePath);
};
