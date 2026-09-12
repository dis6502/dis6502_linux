#pragma once

#include "Control.h"
#include "Memory.h"
#include "Syntax.h"

class EditControl : public Control {
public:
    EditControl(WindowHandle hWnd);

    EditControl() = delete;
    EditControl(const EditControl& a) = delete;			// copy constructor
    EditControl& operator=(const EditControl&) = delete;  // copy assignment
    EditControl(EditControl&&) = delete;                  // move constructor
    EditControl& operator=(EditControl&&) = delete;       // move assignment

    void Clear();
    void SetTextSize(int size);
    void SetText(wstring_view text);
    wstring GetText() const;

    bool HasText() const;
    int GetTextLength() const;

    void SetNumber(unsigned int value);
    unsigned int GetNumber() const;

    void SetByte(byte value);
    byte GetByte() const;

    void SetAddress(Memory::address address);
    Memory::address GetAddress() const;

};
