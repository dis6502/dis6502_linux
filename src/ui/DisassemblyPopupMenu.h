#pragma once

#include "MemoryType.h"
#include "PopupMenu.h"
#include "Syntax.h"

class DIS_LINE;


class DisassemblyPopupMenu : public PopupMenu {
public:
    DisassemblyPopupMenu();

    void Update(
        bool canFindNext, wstring_view labelDefinition, wstring_view labelReference, size_t historyIndex, const DIS_LINE* lpLine, bool instructionHasImmediateOperand, bool charAllowed, MemoryType immediateMemoryType);
};
