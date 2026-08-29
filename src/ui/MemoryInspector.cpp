/*
** MemoryInspector.cpp
**
** Handles memoryInspector window and associated menu.
*/
#include <algorithm>

#include "Application.h"
#include "systems/ComputerSystem.h"
#include "DatatypeUtility.h"
#include "Dialog.h"
#include "FileHeader.h"
#include "FindStringDialog.h"
#include "LowHighByteDialog.h"
#include "Main.h"
#include "MainDisassembly.h"
#include "MainWindow.h"
#include "MemoryInspectorControl.h"
#include "MemoryInspectorControlTypes.h"
#include "MemoryInspectorPopupMenu.h"
#include "MemoryInspectorSelection.h"
#include "MemoryInspectorStack.h"
#include "MemoryInspectorWindow.h"
#include "InstructionSet.h"
#include "Profile.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SelectSpritesDialog.h"
#include "Workspace.h"

#include "MemoryInspector.h"

extern std::unique_ptr<Main> g_Main;
extern std::unique_ptr<Application> g_Application;
extern std::unique_ptr<Workspace> g_Workspace;

extern SegmentList* lpSegmentList;


MemoryInspector::MemoryInspector() :
    popupMenu(nullptr),
    mainWindow(nullptr),
    memoryInspectorWindow(nullptr),
    memoryInspectorControl(nullptr),
    segmentList(nullptr),
    memoryInspectorSelection(*g_Workspace)
{
}

MemoryInspector::~MemoryInspector() {
    if (popupMenu != nullptr) {
        delete popupMenu;
        popupMenu = nullptr;
    }
}

void MemoryInspector::CreateControl() {
    memoryInspectorSelection.Clear();

    popupMenu = new MemoryInspectorPopupMenu();
    popupMenu->CreateControl();
}

void MemoryInspector::Init(MainWindow& mainWindow, MemoryInspectorWindow& memoryInspectorWindow) {
    this->mainWindow = &mainWindow;
    this->memoryInspectorWindow = &memoryInspectorWindow;
    this->memoryInspectorControl = this->memoryInspectorWindow->GetMemoryInspectorControl();
    this->segmentList = ::g_Workspace->GetSegmentList();
    this->mainDisassembly = ::g_Main->GetMainDisassembly();
}

gsl::not_null<const MemoryInspectorSelection*> MemoryInspector::GetConstMemoryInspectorSelection() const {
    return &memoryInspectorSelection;
}

gsl::not_null <MemoryInspectorSelection*> MemoryInspector::GetMemoryInspectorSelection() {
    return &memoryInspectorSelection;
}

void MemoryInspector::Refresh(bool bForceDisassembly) {
    memoryInspectorControl->Refresh();

    ::g_Main->UpdateDisassembly(bForceDisassembly);
}

void MemoryInspector::ShowSelectSpritesDialog() {
    SelectSpritesDialog dialog(*memoryInspectorWindow);

    if (dialog.Show(memoryInspectorSelection)) {
        Memory::offset nBegin, nEnd;
        dialog.GetSelection(nBegin, nEnd);
        Select(nBegin, nEnd);
    }
}

void MemoryInspector::SegmentChanged() {

    // Not all kindes segments can be displayed as memory.
    const bool hasData = (memoryInspectorSelection.segment != nullptr)
        && (!memoryInspectorSelection.segment->IsHeader(FileHeader::SDX_SYM_DEFINED))
        && (!memoryInspectorSelection.segment->IsSDXRelocBlkWithoutData());

    if (hasData) {
        auto size = memoryInspectorSelection.segment->GetSize();
        auto memoryBlock = &memoryInspectorSelection.segment->memoryBlock;
        auto lpType = memoryBlock->GetMutableType()->get();
        memoryInspectorControl->SetBuffer(memoryBlock->GetMutableData()->get(), size);
        memoryInspectorControl->SetTypeBuffer((MemoryType*)lpType, memoryInspectorSelection.segment->wBegin);
        memoryInspectorControl->SetScroll(0);

        memoryInspectorWindow->ClearSelection();
    }
    else {
        memoryInspectorControl->SetBuffer(nullptr, 0);
        memoryInspectorControl->SetTypeBuffer(nullptr, 0);

    }
    memoryInspectorControl->SetScroll(0);

    InvalidateRect(memoryInspectorControl->GetHWnd(), nullptr, true);
}

void MemoryInspector::DrawMenu() {
    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }


    if (IsEditMode()) {
        HMENU hMenuPopup = CreatePopupMenu();

        AppendMenu(hMenuPopup, MF_ENABLED | MF_STRING, IDM_DUMP_QUIT_EDIT, L"Quit memoryInspector edit mode\tEsc");
        // TODO Create popup menu class for this
        POINT pt;
        GetCursorPos(&pt);
        TrackPopupMenu(hMenuPopup, TPM_LEFTALIGN, pt.x, pt.y, 0, mainWindow->GetHWnd(), nullptr);

        DestroyMenu(hMenuPopup);
    }
    else {
        popupMenu->Update(*segmentList, memoryInspectorSelection, CanFind(true), CanFind(false));
        mainDisassembly->SetSelection(memoryInspectorSelection.GetSegmentIndex(), memoryInspectorSelection.GetBegin(), 0xFFFF);

        popupMenu->Track(*mainWindow);
    }
}

/*
** find the first segment from an absolute address
*/
SEGMENT_INDEX MemoryInspector::FindSegmentByAbsoluteAddress(Memory::address wAddr) {
    return segmentList->FindByAddr(wAddr);
}

/*
** find a segment from an absolute address
*/
bool MemoryInspector::MarkOneByte(SEGMENT_INDEX& segmentIndex, Memory::offset& offset, MemoryType memoryType, bool abortIfNotUnknown) {

    const auto& segment = segmentList->GetConstSegment(segmentIndex);
    const auto wBegin = segment->wBegin;
    const auto wEnd = segment->wEnd;

    if ((wBegin + offset) > wEnd) {
        auto nextSegmentIndex = segmentIndex + 1;
        if (nextSegmentIndex >= segmentList->GetCount()) {
            return false;
        }

        const auto& lpNextSegment = segmentList->GetConstSegment(nextSegmentIndex);

        if ((lpNextSegment->IsEmpty()) || (lpNextSegment->bBinary == false)) {
            return false;
        }

        const auto wNextBegin = lpNextSegment->wBegin;

        if (wNextBegin != (wEnd + 1)) {
            return false;
        }

        segmentIndex = nextSegmentIndex;
        offset = 0;
    }

    const auto& markedSegment = segmentList->GetSegment(segmentIndex);

    if (abortIfNotUnknown) {
        if (!markedSegment->IsType(offset, MemoryType::UNKNOWN)) {
            return false;
        }

        markedSegment->SetType(offset, memoryType);
    }
    else {
        if (markedSegment->IsType(offset, MemoryType::UNKNOWN)) {
            markedSegment->SetType(offset, memoryType);
        }
    }

    return true;
}

/*
** fill pLow and pHigh so that the addresses are ordered
** return true if the 2 context have consecutive addresses
*/
bool MemoryInspector::SortContext(DUMP_CONTEXT* OldContext, DUMP_CONTEXT* NewContext, DUMP_CONTEXT** pLow, DUMP_CONTEXT** pHigh, Memory::word* wSrc, Memory::word* wDest) {
    Memory::word wLowOfs;
    Memory::word wLowSegData;
    Memory::word wLowOfsData;
    Memory::word wLowReg;
    byte cLowValue;
    Memory::word wHighOfs;
    Memory::word wHighSegData;
    Memory::word wHighOfsData;
    Memory::word wHighReg;
    byte cHighValue;
    bool bHaveAddress;

    if (OldContext->wAddr < NewContext->wAddr) {
        *pLow = OldContext;
        *pHigh = NewContext;
    }
    else {
        *pLow = NewContext;
        *pHigh = OldContext;
    }

    wLowReg = (*pLow)->Regs[(*pLow)->wIndex].wReg;
    wHighReg = (*pHigh)->Regs[(*pHigh)->wIndex].wReg;
    const auto& computerSystem = ::g_Workspace->GetComputerSystem();

    bHaveAddress = (((*pLow)->wAddr == (Memory::word)((*pHigh)->wAddr - 1)) && (wLowReg != dis_k::NO_DUMP) && (wHighReg != dis_k::NO_DUMP) && (computerSystem->IsBaseAddress((*pLow)->wAddr)));
    if (bHaveAddress) {
        *wSrc = (Memory::word)((wHighReg << 8) + wLowReg);
        *wDest = (*pLow)->wAddr;

        const auto wLowSeg = (*pLow)->Regs[(*pLow)->wIndex].wSeg;
        wLowOfs = (*pLow)->Regs[(*pLow)->wIndex].wOfs;
        const auto wHighSeg = (*pHigh)->Regs[(*pHigh)->wIndex].wSeg;
        wHighOfs = (*pHigh)->Regs[(*pHigh)->wIndex].wOfs;

        if ((wLowSeg != NO_SEGMENT_INDEX) && (wHighSeg != NO_SEGMENT_INDEX)) {
            wLowSegData = (*pLow)->Regs[(*pLow)->wIndex].wSegData;
            wLowOfsData = (*pLow)->Regs[(*pLow)->wIndex].wOfsData;
            wHighSegData = (*pHigh)->Regs[(*pHigh)->wIndex].wSegData;
            wHighOfsData = (*pHigh)->Regs[(*pHigh)->wIndex].wOfsData;

            const auto& lowDataSegment = segmentList->GetSegment(wLowSegData);
            const auto& highDataSegment = segmentList->GetSegment(wHighSegData);
            cLowValue = lowDataSegment->GetData(wLowOfsData);
            cHighValue = highDataSegment->GetData(wHighOfsData);

            const auto& lowSegment = segmentList->GetSegment(wLowSeg);
            const auto& highSegment = segmentList->GetSegment(wHighSeg);

            lowSegment->SetType(wLowOfs, MemoryType::LOBYTE);
            highSegment->SetType(wHighOfs, MemoryType::HIBYTE);
            lowDataSegment->SetData(wLowOfsData, cHighValue);
            highDataSegment->SetData(wHighOfsData, cLowValue);

            OldContext->wAddr = NewContext->wAddr = 0xFFFF;
        }
    }

    return bHaveAddress;
}

/*
** set bytes to DisplayList type
*/
void MemoryInspector::SetDisplayList(Memory::address wAddr) {
    auto segmentIndex = FindSegmentByAbsoluteAddress(wAddr);
    if (segmentIndex != NO_SEGMENT_INDEX) {
        gsl::not_null<const Segment*> lpSegment = segmentList->GetConstSegment(segmentIndex);
        Memory::offset  wOfs = wAddr - lpSegment->wBegin;

        while (MarkOneByte(segmentIndex, wOfs, MemoryType::DLIST)) {
            lpSegment = segmentList->GetConstSegment(segmentIndex);
            const auto bByte = lpSegment->GetData(wOfs);
            wOfs++;

            switch (bByte & 0x0F) {
            case 0: {
                break;
            }

            case 1: {

                const auto wSave = lpSegment->GetWord(wOfs);
                MarkOneByte(segmentIndex, wOfs, MemoryType::DLIST); wOfs++;
                MarkOneByte(segmentIndex, wOfs, MemoryType::DLIST); wOfs++;
                SetDisplayList(wSave);

                return;
            }

            default:
                if (bByte & 0x40) {
                    for (Memory::offset wIndex = 0; wIndex < 2; wIndex++) {
                        MarkOneByte(segmentIndex, wOfs, MemoryType::DLIST);
                        wOfs++;
                    }
                }
                break;
            }
        }
    }
}

/*
** follow the flow of the program to guess byte type
*/
void MemoryInspector::GuessCode() {
    WORD wAddr;
    Memory::word wOpcodeLen;
    byte cOpcode;
    Memory::word wIndex;
    DUMP_REGISTER Reg;
    DUMP_CONTEXT NewContext;
    DUMP_CONTEXT OldContext;
    DUMP_CONTEXT* pLow;
    DUMP_CONTEXT* pHigh;
    Memory::word wSrc;
    Memory::word wDest;
    byte cSaveInst = 0;
    byte bByte;
    bool bNotEnd;
    Memory::word wBytesAsLabels;
    PrintfState nPrintState;

    byte cParam[2] = { 0,0 };
    Memory::word wSave = 0;

    wAddr = memoryInspectorStack.PopAddress();
    const auto& computerSystem = ::g_Workspace->GetComputerSystem();
    while (wAddr != 0xFFFF) {
        auto segmentIndex = FindSegmentByAbsoluteAddress(wAddr);

        if (segmentIndex != NO_SEGMENT_INDEX) {
            const auto& lpSegment = segmentList->GetConstSegment(segmentIndex);
            const auto& instructionSet = g_Workspace->GetInstructionSet(lpSegment->processorType);

            memset(&OldContext, 0xFF, sizeof(OldContext));;
            memset(&NewContext, 0xFF, sizeof(NewContext));
            Memory::offset offset = wAddr - lpSegment->wBegin;

            while (MarkOneByte(segmentIndex, offset, MemoryType::CODE)) {
                Reg.wReg = 0xFFFF;
                Reg.wSeg = segmentIndex;
                Reg.wOfs = offset;

                cOpcode = lpSegment->GetData(offset);
                wOpcodeLen = instructionSet->GetInstruction(cOpcode).GetLength();
                offset++;

                for (wIndex = 1; wIndex < wOpcodeLen; wIndex++) {
                    MarkOneByte(segmentIndex, offset, MemoryType::CODE);
                    cParam[wIndex - 1] = lpSegment->GetData(offset);

                    if (wIndex == 1) {
                        Reg.wReg = (Memory::word)(((Memory::word)cParam[0]) & 0xFF);
                        Reg.wSegData = segmentIndex;
                        Reg.wOfsData = offset;
                    }

                    offset++;
                }

                if (wOpcodeLen == 3) {
                    wSave = (Memory::word)((((Memory::word)cParam[0]) & 0xFF) + (((Memory::word)cParam[1]) << 8));
                }

                if ((cOpcode == 0x20 /*JSR*/) || (cOpcode == 0x4C /*JMP*/)) {
                    // do not push address if it is a SDX symbol
                    if (!lpSegment->IsType(offset - 2, MemoryType::SYMBOL)) {
                        wAddr = (Memory::word)(cParam[0] + (cParam[1] << 8));
                        if (wAddr != 0xFFFF) {
                            memoryInspectorStack.PushAddress(wAddr);
                        }
                    }

                    if (cOpcode == 0x4C) {
                        break;
                    }

                    LPCWSTR szLabel;
                    if ((lpSegment->IsType(offset - 2, MemoryType::SYMBOL)) &&
                        ((szLabel = ::g_Workspace->FindSymbolByAddress(segmentIndex, lpSegment->wBegin + offset - 2, 0))) && String::Equals(szLabel, L"PRINTF")) {
                        nPrintState = PrintfState::TEXT;
                        wBytesAsLabels = 0;

                        while ((bNotEnd = MarkOneByte(segmentIndex, offset, MemoryType::STRING, false))) {
                            bByte = lpSegment->GetData(offset);
                            offset++;

                            if (bByte == 0) {
                                break;
                            }

                            switch (nPrintState) {
                            case PrintfState::TEXT:
                                if (bByte == '%') {
                                    nPrintState = PrintfState::PERCENT;
                                }
                                break;

                            case PrintfState::PERCENT:
                                if (bByte == '%') {
                                    nPrintState = PrintfState::TEXT;
                                }
                                else if (bByte == 'c' || bByte == 's' || bByte == 'p' || bByte == 'x' || bByte == 'b' || bByte == 'd' || bByte == 'e' || bByte == 'l' || bByte == 't') {
                                    nPrintState = PrintfState::TEXT;
                                    wBytesAsLabels += 2;
                                }
                                else {
                                    nPrintState = PrintfState::FORMAT;
                                    wBytesAsLabels += 2;
                                }
                                break;

                            case PrintfState::FORMAT:
                                if (bByte == '*') {
                                    wBytesAsLabels += 2;
                                }
                                else if (bByte == 'c' || bByte == 's' || bByte == 'p' || bByte == 'x' || bByte == 'b' || bByte == 'd' || bByte == 'e' || bByte == 'l' || bByte == 't') {
                                    nPrintState = PrintfState::TEXT;
                                }
                                break;
                            }
                        }

                        if (bNotEnd) {
                            while ((wBytesAsLabels > 0)
                                && (MarkOneByte(segmentIndex, offset, MemoryType::LABEL, false))) {
                                offset++;
                                wBytesAsLabels--;
                            }
                        }
                    }

                    memset(&OldContext, 0xFF, sizeof(OldContext));
                    memset(&NewContext, 0xFF, sizeof(NewContext));
                }
                else if ((cOpcode == 0x10 /*BPL*/) || (cOpcode == 0x30 /*BMI*/) ||
                    (cOpcode == 0x50 /*BVC*/) || (cOpcode == 0x70 /*BVS*/) ||
                    (cOpcode == 0x90 /*BCC*/) || (cOpcode == 0xB0 /*BCS*/) ||
                    (cOpcode == 0xD0 /*BNE*/) || (cOpcode == 0xF0 /*BEQ*/))
                {
                    if (cParam[0] & 0x80)
                        wAddr = (WORD)(lpSegment->wBegin + offset - (256 - (((WORD)cParam[0]) & 0xFF)));
                    else {
                        wAddr = (WORD)(lpSegment->wBegin + offset + cParam[0]);
                    }

                    if (wAddr != 0xFFFF) {
                        memoryInspectorStack.PushAddress(wAddr);
                    }
                }

                if ((cOpcode == 0x00 /*BRK*/) || (cOpcode == 0x40 /*RTI*/) || (cOpcode == 0x60 /*RTS*/) || (cOpcode == 0x6C /*JMP*/))
                    break;

                if ((cOpcode == 0xA9 /*LDA #*/) && (Reg.wReg != 0xFFFF))
                    memcpy(&NewContext.Regs[0], &Reg, sizeof(Reg));
                else if ((cOpcode == 0xA2 /*LDX #*/) && (Reg.wReg != 0xFFFF))
                    memcpy(&NewContext.Regs[1], &Reg, sizeof(Reg));
                else if ((cOpcode == 0xA0 /*LDY #*/) && (Reg.wReg != 0xFFFF))
                    memcpy(&NewContext.Regs[2], &Reg, sizeof(Reg));
                else if (cOpcode == 0xE8 /*INX*/) {
                    if (NewContext.Regs[1].wReg != 0xFFFF)
                        NewContext.Regs[1].wReg = (Memory::word)((NewContext.Regs[1].wReg + 1) & 0xFF);

                    NewContext.Regs[1].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0xCA /*DEX*/) {
                    if (NewContext.Regs[1].wReg != 0xFFFF)
                        NewContext.Regs[1].wReg = (Memory::word)((NewContext.Regs[1].wReg - 1) & 0xFF);

                    NewContext.Regs[1].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0xC8 /*INY*/) {
                    if (NewContext.Regs[2].wReg != 0xFFFF)
                        NewContext.Regs[2].wReg = (Memory::word)((NewContext.Regs[2].wReg + 1) & 0xFF);

                    NewContext.Regs[2].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x88 /*DEY*/) {
                    if (NewContext.Regs[2].wReg != 0xFFFF)
                        NewContext.Regs[2].wReg = (Memory::word)((NewContext.Regs[2].wReg - 1) & 0xFF);

                    NewContext.Regs[2].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x0A /*ASL*/) {
                    if (NewContext.Regs[0].wReg != 0xFFFF)
                        NewContext.Regs[0].wReg = (Memory::word)((NewContext.Regs[0].wReg << 1) & 0xFF);

                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x4A /*LSR*/) {
                    if (NewContext.Regs[0].wReg != 0xFFFF)
                        NewContext.Regs[0].wReg = (Memory::word)((NewContext.Regs[0].wReg >> 1) & 0xFF);

                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x09 /*ORA #*/) {
                    if (NewContext.Regs[0].wReg != 0xFFFF)
                        NewContext.Regs[0].wReg = (Memory::word)((NewContext.Regs[0].wReg | cParam[0]) & 0xFF);

                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x29 /*AND #*/) {
                    if (NewContext.Regs[0].wReg != 0xFFFF)
                        NewContext.Regs[0].wReg = (Memory::word)((NewContext.Regs[0].wReg & cParam[0]) & 0xFF);

                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x49 /*EOR #*/) {
                    if (NewContext.Regs[0].wReg != 0xFFFF)
                        NewContext.Regs[0].wReg = (Memory::word)((NewContext.Regs[0].wReg ^ cParam[0]) & 0xFF);

                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x8A /*TXA*/) {
                    NewContext.Regs[0].wReg = NewContext.Regs[1].wReg;
                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0x98 /*TYA*/) {
                    NewContext.Regs[0].wReg = NewContext.Regs[2].wReg;
                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0xAA /*TAX*/) {
                    NewContext.Regs[1].wReg = NewContext.Regs[0].wReg;
                    NewContext.Regs[1].wSeg = NO_SEGMENT_INDEX;
                }
                else if (cOpcode == 0xA8 /*TAY*/) {
                    NewContext.Regs[2].wReg = NewContext.Regs[0].wReg;
                    NewContext.Regs[2].wSeg = NO_SEGMENT_INDEX;
                }
                else if ((cOpcode == 0x01 /*ORA*/) || (cOpcode == 0x05 /*ORA*/) || (cOpcode == 0x0D /*ORA*/) || (cOpcode == 0x11 /*ORA*/) || (cOpcode == 0x15 /*ORA*/)
                    || (cOpcode == 0x19 /*ORA*/) || (cOpcode == 0x1D /*ORA*/) || (cOpcode == 0x21 /*AND*/) || (cOpcode == 0x25 /*AND*/) || (cOpcode == 0x2A /*ROL*/)
                    || (cOpcode == 0x2D /*AND*/) || (cOpcode == 0x31 /*AND*/) || (cOpcode == 0x35 /*AND*/) || (cOpcode == 0x39 /*AND*/) || (cOpcode == 0x3D /*AND*/)
                    || (cOpcode == 0x41 /*EOR*/) || (cOpcode == 0x45 /*EOR*/) || (cOpcode == 0x4D /*EOR*/) || (cOpcode == 0x51 /*EOR*/) || (cOpcode == 0x55 /*EOR*/)
                    || (cOpcode == 0x59 /*EOR*/) || (cOpcode == 0x5D /*EOR*/) || (cOpcode == 0x61 /*ADC*/) || (cOpcode == 0x65 /*ADC*/) || (cOpcode == 0x68 /*PLA*/)
                    || (cOpcode == 0x6A /*ROR*/) || (cOpcode == 0x6D /*ADC*/) || (cOpcode == 0x71 /*ADC*/) || (cOpcode == 0x75 /*ADC*/) || (cOpcode == 0x79 /*ADC*/)
                    || (cOpcode == 0x7D /*ADC*/) || (cOpcode == 0xA1 /*LDA*/) || (cOpcode == 0xA5 /*LDA*/) || (cOpcode == 0xAD /*LDA*/) || (cOpcode == 0xB1 /*LDA*/)
                    || (cOpcode == 0xB5 /*LDA*/) || (cOpcode == 0xB9 /*LDA*/) || (cOpcode == 0xBD /*LDA*/) || (cOpcode == 0xE1 /*SBC*/) || (cOpcode == 0xE5 /*SBC*/)
                    || (cOpcode == 0xE9 /*SBC*/) || (cOpcode == 0xED /*SBC*/) || (cOpcode == 0xF1 /*SBC*/) || (cOpcode == 0xF5 /*SBC*/) || (cOpcode == 0xF9 /*SBC*/)
                    || (cOpcode == 0xFD /*SBC*/))
                {
                    NewContext.Regs[0].wReg = (Memory::word)0xFFFF;
                    NewContext.Regs[0].wSeg = NO_SEGMENT_INDEX;
                }
                else if ((cOpcode == 0xA6 /*LDX*/) || (cOpcode == 0xAE /*LDX*/) || (cOpcode == 0xB6 /*LDX*/) || (cOpcode == 0xBA /*TSX*/) || (cOpcode == 0xBE /*LDX*/)) {
                    NewContext.Regs[1].wReg = (Memory::word)0xFFFF;
                    NewContext.Regs[1].wSeg = NO_SEGMENT_INDEX;
                }
                else if ((cOpcode == 0xA4 /*LDY*/) || (cOpcode == 0xAC /*LDY*/) || (cOpcode == 0xB4 /*LDY*/) || (cOpcode == 0xBC /*LDY*/)) {
                    NewContext.Regs[2].wReg = (Memory::word)0xFFFF;
                    NewContext.Regs[2].wSeg = NO_SEGMENT_INDEX;
                }
                else if ((cOpcode == 0x85 /*STA Zpg*/) && (NewContext.Regs[0].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = (Memory::word)(cParam[0] & 0xFF);
                    NewContext.wIndex = 0;
                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x8D /*STA Abs*/) && (NewContext.Regs[0].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = wSave;
                    NewContext.wIndex = 0;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x9D /*STA Abs,X*/) && (NewContext.Regs[0].wReg != 0xFFFF) && (NewContext.Regs[1].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = (WORD)(wSave + NewContext.Regs[1].wReg);
                    NewContext.wIndex = 0;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x99 /*STA Abs,Y*/) && (NewContext.Regs[0].wReg != 0xFFFF) && (NewContext.Regs[2].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = (WORD)(wSave + NewContext.Regs[2].wReg);
                    NewContext.wIndex = 0;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x86 /*STX Zpg*/) && (NewContext.Regs[1].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = (WORD)(cParam[0] & 0xFF);
                    NewContext.wIndex = 1;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x8E /*STX Abs*/) && (NewContext.Regs[1].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = wSave;
                    NewContext.wIndex = 1;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x84 /*STY Zpg*/) && (NewContext.Regs[2].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = (cParam[0] & 0xFF);
                    NewContext.wIndex = 2;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if ((cOpcode == 0x8C /*STY Abs*/) && (NewContext.Regs[2].wReg != 0xFFFF) && (cOpcode != cSaveInst)) {
                    NewContext.wAddr = wSave;
                    NewContext.wIndex = 2;

                    if ((OldContext.wAddr != 0xFFFF) && (SortContext(&OldContext, &NewContext, &pLow, &pHigh, &wSrc, &wDest))) {
                        if (computerSystem->IsDisplayListVectorAddress(wDest)) {
                            SetDisplayList(wSrc);
                        }
                        else if (computerSystem->IsVectorAddress(wDest)) {
                            memoryInspectorStack.PushAddress(wSrc);
                        }
                    }

                    memcpy(&OldContext, &NewContext, sizeof(OldContext));
                    NewContext.wAddr = 0xFFFF;
                }
                else if (instructionSet->GetInstruction(cOpcode).IsUnsupportedInstruction()) {
                    memset(&OldContext, 0xFF, sizeof(OldContext));
                    memset(&NewContext, 0xFF, sizeof(NewContext));
                }

                cSaveInst = cOpcode;
            }
        }

        wAddr = memoryInspectorStack.PopAddress();
    }
}

/*
** guess byte type
*/
void MemoryInspector::Guess() {

    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    if (memoryInspectorSelection.segment->bBinary == false) {
        return;
    }

    memoryInspectorStack.PushAddress((Memory::address)(memoryInspectorSelection.segment->wBegin + memoryInspectorSelection.GetBegin()));
    GuessCode();

    memoryInspectorStack.Clear();
    Refresh();
}

void MemoryInspector::SetUnknownBlockToByte() {
    Memory::word wBegin;
    Memory::size nSize;
    Memory::word wIndex;

    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    if (memoryInspectorSelection.segment->bBinary == false) {
        return;
    }

    wBegin = memoryInspectorSelection.GetBegin();
    nSize = memoryInspectorSelection.GetSize();

    for (wIndex = wBegin; wIndex < (wBegin + nSize); wIndex++) {
        if (memoryInspectorSelection.segment->IsType(wIndex, MemoryType::UNKNOWN) // TODO: This is a recurring check, centralize it
            && ((wIndex == 0) || ((memoryInspectorSelection.segment->IsType(wIndex - 1, MemoryType::LOBYTE))
                && (memoryInspectorSelection.segment->IsType(wIndex - 1, MemoryType::HIBYTE)))))
        {
            memoryInspectorSelection.segment->SetType(wIndex, MemoryType::BYTE);
        }
    }

    Refresh();
}

void MemoryInspector::SelectNextUnknownBlock() {

    if (memoryInspectorSelection.segment == nullptr) {
        return;
    }

    if (memoryInspectorSelection.segment->IsEmpty()) {
        return;
    }

    if (memoryInspectorSelection.segment->bBinary == false) {
        return;
    }

    Memory::offset wLast = 0;
    if (memoryInspectorSelection.HasSelection()) {
        wLast = memoryInspectorSelection.GetBegin() + 1;
    }

    const auto count = segmentList->GetCount();
    for (auto segmentIndex = memoryInspectorSelection.GetSegmentIndex(); segmentIndex < count; segmentIndex++) {
        const auto& segment = segmentList->GetConstSegment(segmentIndex);
        Memory::offset wEnd = segment->wEnd - segment->wBegin;

        for (Memory::offset offset = wLast; offset <= wEnd; offset++) {
            if (segment->IsUnknown(offset)) {
                if (memoryInspectorSelection.GetSegmentIndex() != segmentIndex) {
                    memoryInspectorSelection.SetSegmentIndex(segmentIndex);
                    segmentList->SetSelectedIndex(segmentIndex);
                }

                const auto wBegin = offset;
                wLast = wBegin;
                for (offset = wBegin + 1; ((offset <= wEnd) && (segment->IsUnknown(offset))); offset++) {
                    wLast = offset;
                }

                wEnd = wLast;
                Select(wBegin, wEnd);

                return;
            }
        }

        wLast = 0;
    }
}

void MemoryInspector::SelectionChanged() {
    Memory::offset nBegin, nEnd;

    if (memoryInspectorControl->GetRawSelection(nBegin, nEnd)) {
        memoryInspectorSelection.SetSelection(nBegin, nEnd);

        const auto editMode = IsEditMode();
        int offset = ((nBegin <= nEnd) || (editMode)) ? nBegin : nEnd; // Must be signed!
        auto found = false;

        // Start at the offset and count down until a proper start position is recognized by the disassembly window.
        while ((!found) && (offset >= 0)) {
            found = mainDisassembly->SelectLineWithInfo(memoryInspectorSelection.GetSegmentIndex(), offset);
            offset--;
        }

        // If start position was found, try the same of the end position.
        if ((found) && (nBegin != nEnd) && (!editMode)) {
            found = false;
            offset = (nBegin <= nEnd) ? nEnd : nBegin;

            while ((!found) && (offset > 0)) {
                found = mainDisassembly->ExtendSelectionTo(memoryInspectorSelection.GetSegmentIndex(), offset);
                offset--;
            }
        }
    }
    else {
        memoryInspectorSelection.ClearSelection();
    }
}

void MemoryInspector::Disassemble() {
    mainDisassembly->Process(*this, memoryInspectorSelection.segment);
}

bool MemoryInspector::AskByte(MemoryType bType, byte cKnownByte, byte& unknownByte) {
    LowHighByteDialog dialog(*mainWindow);

    const auto result = dialog.Show(bType, cKnownByte);
    if (result) {
        unknownByte = dialog.GetUnknownByte();
    }

    return result;
}

void MemoryInspector::SetType(MemoryType bType) {

    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    if (memoryInspectorSelection.segment->bBinary == false) {
        return;
    }

    const auto segment = memoryInspectorSelection.segment;
    const auto wBegin = memoryInspectorSelection.GetBegin();
    const auto wSize = memoryInspectorSelection.GetSize();

    if ((bType == MemoryType::LOBYTE) || (bType == MemoryType::HIBYTE)) {
        if (wBegin == 0) {
            ::g_Application->SendErrorMessageWithID(IDS_ERR_LOHI_FIRST);
            return;
        }

        if (wSize != 1) {
            ::g_Application->SendErrorMessageWithID(IDS_ERR_MULTI_LOHI);
            return;
        }

        const auto previousOpcode = segment->GetData(wBegin - 1);
        const auto& instructionSet = g_Workspace->GetInstructionSet(segment->processorType);
        if (instructionSet->GetInstruction(previousOpcode).GetOperandMode() != OperandMode::Immediate) {
            ::g_Application->SendErrorMessageWithID(IDS_ERR_BAD_MODE_FOR_LOHI);
            return;
        }

        byte unknownByte;
        if (AskByte(bType, segment->GetData(wBegin), unknownByte) == false) {
            return;
        }

        // The type of the immediate opcode before the known byte is set to MemoryType::LOBYTE or MemoryType::LOBYTE
        // The user input for the unknown byte in the assumed word is stored in the type of the operand.
        segment->SetType(wBegin - 1, bType);
        segment->SetType(wBegin, (MemoryType)unknownByte);
    }
    else {
        if (wBegin > 0) {
            if ((segment->IsType(wBegin - 1, MemoryType::LOBYTE))
                || (segment->IsType(wBegin - 1, MemoryType::HIBYTE)))
                segment->SetType(wBegin - 1, MemoryType::CODE);

            if ((segment->IsType(wBegin + wSize - 1, MemoryType::LOBYTE))
                || (segment->IsType(wBegin + wSize - 1, MemoryType::HIBYTE))
                /* && not at the end of buffer */)
                segment->SetType(wBegin + wSize, MemoryType::CODE);
        }

        segment->SetType(wBegin, bType, wSize);
    }

    Refresh();
}

void MemoryInspector::Reset() {
    memoryInspectorControl->SetBuffer(nullptr, 0);
    memoryInspectorControl->SetTypeBuffer(nullptr, 0);
    InvalidateRect(memoryInspectorControl->GetHWnd(), nullptr, true);

    memoryInspectorSelection.Clear();
}

void MemoryInspector::ToggleDisplayAsScreenCode() {
    ::g_Workspace->SetViewDisplayAsScreenCode(!::g_Workspace->IsViewDisplayAsScreenCode());
    memoryInspectorControl->SetInternal(::g_Workspace->IsViewDisplayAsScreenCode());
}

bool MemoryInspector::HasFindString() const {
    return !findString.empty();
}

bool MemoryInspector::FindString(wstring_view findAscii, bool allSegments) {
    if (allSegments) {
        finder.segmentIndex = 0;
    }
    else {
        finder.segmentIndex = memoryInspectorSelection.GetSegmentIndex();
    }

    findString = findAscii;

    finder.offset = 0;
    finder.size = findAscii.length();;
    finder.bFindWhole = allSegments;

    return FindNextString();
}

bool MemoryInspector::SearchString(SEGMENT_INDEX segmentIndex, const Segment& segment) {
    const auto size = segment.GetSize();
    if (size >= finder.size) {
        // Search is always on byte level
        const auto findBytes = String::wstring_to_ansi(findString);
        const byte firstByte = findBytes[0];
        for (auto nOffset = finder.offset; nOffset <= (size - finder.size); nOffset++) {
            auto data = segment.GetData(nOffset);
            if (data == firstByte) {
                if (memcmp(&segment.memoryBlock.lpData[nOffset], findBytes.c_str(), finder.size) == 0) {
                    const Memory::offset nBegin = nOffset;
                    const Memory::offset nEnd = nOffset + finder.size - 1;

                    finder.segmentIndex = segmentIndex;
                    finder.offset = nOffset + 1;

                    if (finder.segmentIndex != memoryInspectorSelection.GetSegmentIndex()) {
                        segmentList->SetSelectedIndex(finder.segmentIndex);
                    }

                    Select(nBegin, nEnd);

                    return true;
                }
            }
        }
    }
    return false;
}

bool MemoryInspector::FindNextString() {
    if (finder.segmentIndex != NO_SEGMENT_INDEX) {
        const auto count = segmentList->GetCount();
        for (SEGMENT_INDEX segmentIndex = finder.segmentIndex; segmentIndex < count; segmentIndex++) {
            const auto& segment = segmentList->GetConstSegment(segmentIndex);

            if (SearchString(segmentIndex, *segment)) {
                return true;
            }

            if (finder.bFindWhole == 0) {
                break;
            }

            finder.offset = 0;
        }
    }

    FindStringDialog::ShowStringNotFoundMessage(mainWindow, findString);

    return false;
}

bool MemoryInspector::CanFind(bool first) const {
    if (memoryInspectorSelection.segment == nullptr) {
        return false;
    }

    if ((first == false) && (finder.size == 0)) {
        return false;
    }

    return true;
}

void MemoryInspector::GetFindParameters(wstring& findString, bool& whole) const {
    findString = this->findString;
    whole = finder.bFindWhole;
}

void MemoryInspector::SelectAll() {

    if (memoryInspectorSelection.segment == nullptr) {
        return;
    }

    if (memoryInspectorSelection.segment->IsEmpty()) {
        return;
    }

    constexpr Memory::address wBegin = 0;
    const Memory::address wEnd = memoryInspectorSelection.segment->GetSize();

    Select(wBegin, wEnd);
}

void MemoryInspector::ClearSelection() {
    Select(DUMP_NO_SELECTION, DUMP_NO_SELECTION);
}

void MemoryInspector::Select(Memory::offset wBegin, Memory::offset wEnd) {
    if (memoryInspectorSelection.segment == nullptr) {
        return;
    }

    if (memoryInspectorSelection.segment->IsEmpty()) {
        return;
    }

    if ((wBegin == wEnd) && (wBegin == DUMP_NO_SELECTION)) {
        memoryInspectorSelection.ClearSelection();
    }
    else {
        memoryInspectorSelection.SetSelection(wBegin, wEnd);
    }

    memoryInspectorControl->SetSelection(wBegin, wEnd);
}

void MemoryInspector::DeleteSelection() {

    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    const auto selectionBegin = memoryInspectorSelection.GetBegin();
    const auto selectionSize = memoryInspectorSelection.GetSize();

    const auto remainingSize = memoryInspectorSelection.segment->GetSize() - selectionSize;
    if (remainingSize > 0) {
        memmove(&memoryInspectorSelection.segment->memoryBlock.lpData[selectionBegin], &memoryInspectorSelection.segment->memoryBlock.lpData[selectionBegin + selectionSize], remainingSize);
        memmove(&memoryInspectorSelection.segment->memoryBlock.lpType[selectionBegin], &memoryInspectorSelection.segment->memoryBlock.lpType[selectionBegin + selectionSize], remainingSize);

        memoryInspectorSelection.segment->wEnd -= selectionSize; // TODO: This does not reduce the actually allocated segment size
    }

    // If last byte in memoryInspector segment is deleted, delete also the segment.
    const auto& segmentList = ::g_Workspace->GetSegmentList();
    if (memoryInspectorSelection.segment->IsEmpty()) {
        segmentList->DeleteSelectedSegment();
    }
    else {
        segmentList->NotifySegmentContentChanged();
    }
}

void MemoryInspector::CopySelection() {
    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    const auto begin = memoryInspectorSelection.GetBegin();
    const auto size = memoryInspectorSelection.GetSize();
    auto valueArray = &memoryInspectorSelection.segment->memoryBlock.lpData[begin];
    auto text = DatatypeUtility::ByteArrayToHexString(valueArray, size, false);
    ::g_Application->SetClipboardText(text);
}

void MemoryInspector::PasteAtSelection(bool after) { // TODO: This does not work currently, because CopySelection was changed from binary to hex string

    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    if (OpenClipboard(mainWindow->GetHWnd())) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData) {
            bool bNull = false;

            auto clipboardSize = (Memory::size)GlobalSize(hData); // TODO: Check actual global size against max
            auto data = (byte*)GlobalLock(hData);

            for (size_t offset = 0; offset < clipboardSize - 1; offset++) {
                if (data[offset] == 0) {
                    bNull = true;
                }
            }

            if ((clipboardSize) && (data[clipboardSize - 1] == 0) && (bNull == false)) {
                clipboardSize--;
            }

            auto segment = memoryInspectorSelection.segment;
            const auto size = segment->GetSize();

            // TODO: Put the following to class Segment/Content
            const size_t newSize = size + clipboardSize;
            byte* newData = new byte[newSize];
            MemoryType* newType = new MemoryType[newSize];

            auto wBegin = memoryInspectorSelection.GetBegin();
            if (wBegin > 0) {
                memcpy(newData, segment->memoryBlock.GetData()->getConst(), wBegin);
                memcpy(newType, segment->memoryBlock.GetType()->getConst(), wBegin);
            }

            memcpy(&newData[wBegin], data, clipboardSize);

            if (size - wBegin) {
                memcpy(&newData[wBegin + clipboardSize], &segment->memoryBlock.lpData[wBegin], size - wBegin);
                memcpy(&newType[wBegin + clipboardSize], &segment->memoryBlock.lpType[wBegin], size - wBegin);
            }

            // TODO: delete segment->memoryBlock.lpData;
            // TODO: delete segment->memoryBlock.lpType;
            // TODO: segment->memoryBlock.lpData = newData;
            // TODO: segment->memoryBlock.lpType = newType;

            segment->wEnd += clipboardSize;

            GlobalUnlock(hData);

            g_Workspace->GetSegmentList()->NotifySegmentContentChanged();
        }

        CloseClipboard();
    }
}

void MemoryInspector::SplitAtSelection() {
    if (memoryInspectorSelection.IsEmpty()) {
        return;
    }

    g_Workspace->GetSegmentList()->SplitSelectedSegment(
        memoryInspectorSelection.GetBegin());
}

bool MemoryInspector::SetEditMode(bool editMode) {
    if (editMode && memoryInspectorSelection.IsEmpty()) {
        return false;
    }

    const auto oldEditMode = IsEditMode();

    if (editMode) {
        Memory::offset nBegin, nEnd;

        if (memoryInspectorControl->GetRawSelection(nBegin, nEnd)) {
            memoryInspectorControl->SetSelection(nBegin, nBegin);
            memoryInspectorSelection.SetSelection(nBegin, nBegin);
        }
        else {
            memoryInspectorSelection.ClearSelection();
            memoryInspectorControl->ClearSelection();
        }
    }

    memoryInspectorControl->SetEditMode(editMode);

    return oldEditMode;
}

bool MemoryInspector::IsEditMode() {
    return memoryInspectorControl->IsEditMode();
}
