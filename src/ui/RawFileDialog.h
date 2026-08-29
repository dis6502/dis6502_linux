#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "Memory.h"
#include "Byte.h"
#include "ByteArray.h"

class MemoryInspectorControl;


class RawFileDialog : public Dialog {
public:
    RawFileDialog(const Window& parentWindow, HFONT hComputerFont);
    ~RawFileDialog() override;

    INT_PTR Show(wstring_view filePath);
    ByteArray GetFileBuffer() const;
    void GetResult(Memory::offset& nBegin, Memory::size& nSize, Memory::address& wAddr) const;

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

    void CreateControls() override;
    void DeleteControls() override;

private:
    // Set in constructor.
    HFONT hComputerFont;

    // Set in Show().
    wstring filePath;
    ByteArray fileBuffer;

    // Set in dialog.
    std::unique_ptr<MemoryInspectorControl> memoryInspectorControl;

    Memory::offset nBegin;
    Memory::size nSize;
    Memory::address wAddr;

    void Clear();
};
