#pragma once

#include "Syntax.h"
#include "PopupMenu.h"
#include "MemoryType.h"

class DIS_LINE;


class DisassemblyPopupMenu : public PopupMenu {
public:
    DisassemblyPopupMenu();

    void Update(
        bool canFindNext, wstring_view labelDefinition, wstring_view labelReference, DWORD hisotryIndex, const DIS_LINE* lpLine, bool instructionHasImmediateOperand, bool charAllowed, MemoryType immediateMemoryType);
};
