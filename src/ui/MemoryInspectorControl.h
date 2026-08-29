/*
* Wrapper class for MemoryInspectorControl.
*
* This class wrapps the methods implemented as messages in the MemoryInspectorControl class.
*/
#pragma once

#include "Syntax.h"

#include "Memory.h"
#include "MemoryType.h"
#include "Window.h"


class MemoryInspectorControl : public Window {
public:
    static void RegisterClassFor(HINSTANCE hInstance);

    // Constructor. After the constructor either CreateControl() or BindControl() must be called
    MemoryInspectorControl(const Window& parentWindow, ChildID childID);

    void CreateControl(int x, int y, int nWidth, int nHeight);
    void BindControl();

    void SetNumberOfBytesPerLine(int nNumberOfBytesPerLine);
    void SetInternal(bool bInternal);
    void SetBuffer(byte* lpBuffer, unsigned int nBufferSize);
    void SetTypeBuffer(MemoryType* lpTypeBuffer, Memory::address startAddress);
    void Refresh();
    void SetScroll(UINT nLine);

    void ClearSelection();
    bool GetRawSelection(Memory::offset& wBegin, Memory::offset& wEnd) const; // wBegin may be greater than wEnd
    bool GetSelection(Memory::offset& wBegin, Memory::offset& wEnd, bool bDefaultAll = false) const; // wBegin is always less or equal to wEnd
    void GetNonEmptySelection(Memory::offset& wBegin, Memory::offset& wEnd) const;
    void SetSelection(Memory::offset wBegin, Memory::offset wEnd);

    bool IsEditMode() const;
    void SetEditMode(bool bEditMode);

private:
    ChildID childID; // TODO: What for?
    UINT nBufferSize;
};
