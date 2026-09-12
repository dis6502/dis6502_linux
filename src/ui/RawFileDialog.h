#pragma once

#include "ByteArray.h"
#include "Dialog.h"
#include "Font.h"
#include "Memory.h"
#include "Syntax.h"
#include "Window.h"
#include <memory>

class MemoryInspectorControl;


class RawFileDialog : public Dialog {
public:
    RawFileDialog(const Window& parentWindow, Font* computerFont);
    ~RawFileDialog() override;

    INT_PTR Show(wstring_view filePath);
    ByteArray GetFileBuffer() const;
    void GetResult(Memory::offset& nBegin, Memory::size& nSize, Memory::address& wAddr) const;

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

    void CreateControls() override;
    void DeleteControls() override;

private:
    // Set in constructor.
    Font* computerFont;

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
