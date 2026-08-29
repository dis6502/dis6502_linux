#pragma once

#include "Syntax.h"

#include "Memory.h"
#include "MemoryType.h"
#include "MemoryInspectorSelection.h"
#include "MemoryInspectorStack.h"
#include "SegmentTypes.h"

#include <gsl/pointers>

class MemoryInspector;
class MemoryInspectorWindow;
class MemoryInspectorControl;
class MemoryInspectorPopupMenu;
class SelectSpritesDialog;
class MainWindow;
class MainDisassembly;
class Workspace;
class SegmentList;
class Window;


class Finder {
public:
    static const int MAX_CHARS = 64;
    SEGMENT_INDEX segmentIndex = NO_SEGMENT_INDEX;
    Memory::offset offset = 0;
    Memory::size size = 0;
    bool bFindWhole = true;
};


class MemoryInspector {
public:
    struct DUMP_REGISTER {
    public:
        WORD                     wReg = 0;
        SEGMENT_INDEX            wSeg = 0;
        WORD                     wOfs = 0;
        WORD                     wSegData = 0;
        WORD                     wOfsData = 0;
    };

    struct DUMP_CONTEXT {
    public:
        DUMP_REGISTER            Regs[3];
        WORD                     wIndex;
        WORD                     wAddr;
    };

    MemoryInspector();
    virtual ~MemoryInspector();

    void CreateControl();
    void Init(MainWindow& mainWindow, MemoryInspectorWindow& memoryInspectorWindow);

    gsl::not_null<const MemoryInspectorSelection*> GetConstMemoryInspectorSelection() const;
    gsl::not_null<MemoryInspectorSelection*> GetMemoryInspectorSelection();

    void Refresh(bool forceDisassembly = false);
    void SegmentChanged();
    void DrawMenu();
    void SelectionChanged();
    void ShowSelectSpritesDialog();
    void Disassemble();
    void SetType(MemoryType bType);
    void Reset();
    void ToggleDisplayAsScreenCode();

    bool HasFindString() const;
    bool FindString(wstring_view findAscii, bool allSegments);
    bool FindNextString();
    bool CanFind(bool first) const;
    void GetFindParameters(wstring& findString, bool& whole) const;

    void SelectAll();
    void ClearSelection();
    void Select(Memory::offset wBegin, Memory::offset wEnd);
    void SetUnknownBlockToByte();
    void Guess();
    void SelectNextUnknownBlock();
    void DeleteSelection();
    void CopySelection();
    void PasteAtSelection(bool after);
    void SplitAtSelection();
    bool SetEditMode(bool editMode);
    bool IsEditMode();

private:
    friend class MemoryInspectorSelection;

    MemoryInspectorPopupMenu* popupMenu;

    MainWindow* mainWindow; // TODO: Event handling should be on the MemoryInspectorWindow instead
    MemoryInspectorWindow* memoryInspectorWindow;
    MemoryInspectorControl* memoryInspectorControl;
    MainDisassembly* mainDisassembly;

    SegmentList* segmentList;

    MemoryInspectorStack memoryInspectorStack;
    MemoryInspectorSelection memoryInspectorSelection;

    Finder finder;
    wstring findString;

    SEGMENT_INDEX FindSegmentByAbsoluteAddress(Memory::address wAddr);
    bool MarkOneByte(SEGMENT_INDEX& segmentIndex, Memory::offset& offset, MemoryType memoryType, bool abortIfNotUnknown = true);
    bool SortContext(DUMP_CONTEXT* OldContext, DUMP_CONTEXT* NewContext, DUMP_CONTEXT** pLow, DUMP_CONTEXT** pHigh, Memory::word* wSrc, Memory::word* wDest);
    void SetDisplayList(Memory::address wAddr);
    void GuessCode();
    bool SearchString(SEGMENT_INDEX segmentIndex, const Segment& segment);
    bool AskByte(MemoryType memoryType, byte knownByte, byte& unknownByte);
};
