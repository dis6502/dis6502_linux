#pragma once

#include "Syntax.h"
#include "MainController.h"
#include "DisassemblyLine.h"
#include "DisassemblyResult.h"
#include "DisassemblySection.h"
#include "MemoryInspector.h" // TODO: Should not be a direct usage

class DisassemblyControl;
class MainXRef;

class MainDisassembly final : public MainController {
public:
    MainDisassembly(Main& main);
    void SetControl();
    void SetMainXRef(MainXRef& mainXRef);

    long Proc(HWND hWnd, WPARAM wParam, LPARAM lParam);
    void PerformCommands(WORD wCommand, bool bEditMode);

    void AddComment();
    void SetSelection(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size);
    void SelectLine(DisassemblyResult::LineNumber lineNumber);
    bool SelectLineWithInfo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);
    bool ExtendSelectionTo(SEGMENT_NUMBER segmentNumber, Memory::offset offset);

    // Start the disassembly by opening the dialog box that shows the process.
    void Process(MemoryInspector& memoryInspector, const Segment* lpSegment);

    void RefreshXRef(wstring_view findString);
    // Position to source code line with xrefLineNumber = wLine
    void XRefSelected(XRef::LineNumber xrefLineNumber);

private:
    DisassemblyControl* disassemblyControl;
    MainXRef* mainXRef;

    // Selection state.
    struct DisSelection {
    public:
        SEGMENT_INDEX segmentIndex = NO_SEGMENT_INDEX;
        Memory::offset offset = 0;
        Memory::size size = 0;
    };
    DisSelection disSelection;

    // Find state.
    DisassemblyResult::LineNumber findFirstLineNumber;
    wstring  findString;

    void RefreshDisControl();
    void ResetDisControl();

    void BackInHistory();

    void DrawMenu(const DIS_LINE* lpLine);

    void Find();
    void FindDef();
    void FindRef1();
    void FindRef2();

    void RenameDef();
    void RenameRef();
    void AddrRangeDef();
    void AddrRangeRef();

    bool CanFind(bool bFirst) const;
    wstring GetFindString() const;
    bool FindString(wstring_view findString);
    bool FindNextString(bool first);

    bool AskByte(MemoryType bType, byte cKnownByte, byte &unknownByte);
    bool SetImmediateType(MemoryType immType);
};