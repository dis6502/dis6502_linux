/*
* Disassembly.cpp
* Implements one disasseblmy run.
*/

#include "Disassembly.h"
#include "PlatformCompat.h"

#include "DatatypeUtility.h"
#include <algorithm>
#include <string.h>

#include "Application.h"
#include "Debug.h"
#include "DisassemblyLine.h"
#include "DisassemblyProgressMonitor.h"
#include "DisassemblyResultIterators.h"
#include "DisassemblyResultTest.h"
#include "DisassemblyWriter.h"

#include "Fixup.h"
#include "EquateList.h"
#include "Memory.h"
#include "AddressLabelList.h"
#include "AddressLabel.h"
#include "MemoryBlockIterator.h"
#include "Profile.h"
#include "InstructionSet.h"
#include "Segment.h"
#include "SegmentList.h"
#include "Strings.h"
#include "Workspace.h"
#include "Pass1.h"


Disassembly::Disassembly() :lineWriter(1024) {

    disassemblyProgressMonitor = nullptr;
}

void Disassembly::SetWorkspace(Workspace& workspace) {
    this->workspace = &workspace;
    this->profile = workspace.GetConstProfile();
    this->result = workspace.GetDisassemblyResult();
}

void Disassembly::SetProgressMonitor(DisassemblyProgressMonitor& disassemblyProgressMonitor) {
    this->disassemblyProgressMonitor = &disassemblyProgressMonitor;
}

void Disassembly::StartDisassembly() {
    disassemblyProgressMonitor->StartDisassembly(*this);
}

/*
** Add a line into a buffer.
*/
void Disassembly::AddLineInBuffer(const DIS_LINE& disLine, const wchar_t* szLine, DIS_BUFFER* pBuf) { // TODO: This is the last reference to DIS_BUFFER outside of the DisassemblyResult


    // TODO: The following line should be in the result
    pBuf->AddLine(disLine, szLine);

    // Transition to a different segement=
    if (markSegmentIndex != markNextSegmentIndex) {
        if (markSegmentIndex == NO_SEGMENT_INDEX) {
            markOffset = 0;
            markSegmentIndex = markNextSegmentIndex;
        }
        else {
            const auto& markSegment = workspace->GetConstSegment(markSegmentIndex);
            const auto markSegmentSize = markSegment->GetSize();

            if (markOffset + markSize >= markSegmentSize) {
                if ((markNextSegmentIndex != NO_SEGMENT_INDEX) && (markSegmentSize > 0)) {
                    markOffset = markOffset + markSize - markSegmentSize;
                }
                else {
                    markOffset = dis_k::NO_DUMP;
                }

                markSegmentIndex = markNextSegmentIndex;
            }
            else {
                markOffset += markSize;
            }
        }
    }
    else {
        markOffset += markSize;
    }

    markSize = 0;
}

void Disassembly::AddEmptyCommentLine(DisassemblySectionType disassemblySectionType) {
    AddLine(profile->commentPrefix.c_str(), disassemblySectionType);
}

/*
** Add a label definition into buffer. The following global state variables are used:
*  - markSegmentIndex, markOffset, markIndex
*  - absoluteAddress, systemAddress
*/
void Disassembly::AddLine(const wchar_t* szLine, DisassemblySectionType disassemblySectionType) {
    wchar_t szComment[40]{};


    // Ensure this method is only used in Pass4/5/6 
    if (pass != 4 && pass != 5 && pass != 6) {
        throw std::runtime_error("AddLine() must only be called in pass 4/5/6.");
    }

    // Allocate memory for section if not already done.
    bool newSection = false;
    auto section = result->GetSection(disassemblySectionType);
    DIS_BUFFER* pBuf = nullptr;
    if (section == nullptr) {
        // Allocate a first buffer.
        newSection = true;
        section = result->AllocSection(disassemblySectionType);
        pBuf = section->AllocBuffer();
    }
    else {
        pBuf = section->FindLastBuffer();
    }

    auto disLine = DIS_LINE(*section);

    disLine.segmentIndex = markSegmentIndex;
    disLine.offset = markOffset;
    disLine.size = markSize;
    disLine.address = absoluteAddress;
    disLine.systemAddress = systemAddress;

    if (markSegmentIndex != NO_SEGMENT_INDEX) {
        const auto& markSegment = workspace->GetConstSegment(markSegmentIndex);
        const auto markSegmentSize = markSegment->GetSize();
        if (markOffset + markSize > markSegmentSize) {
            disLine.size = markSegmentSize - markOffset;
        }
    }

    if (newSection) {

        // Add the comment header.
        const auto oldAbsoluteAddress = absoluteAddress;
        const auto oldSystemAddress = systemAddress;
        systemAddress = 0;
        auto text = DisassemblySection::GetText(disassemblySectionType);

        switch (disassemblySectionType) {
        case DisassemblySectionType::SYSTEM_EQUATES:
        case DisassemblySectionType::USER_EQUATES:
        case DisassemblySectionType::CODE_EQUATES:
            markNextSegmentIndex = NO_SEGMENT_INDEX;
            markSegmentIndex = NO_SEGMENT_INDEX;
            markOffset = 0xFFFF;
            markSize = 0;
            break;

        case DisassemblySectionType::CODE_LINES:
            break;
        }
        AddLineInBuffer(disLine, profile->commentPrefix.c_str(), pBuf);
        wsprintf(szComment, L"%s %s", profile->commentPrefix.c_str(), text.c_str());
        AddLineInBuffer(disLine, szComment, pBuf);
        AddLineInBuffer(disLine, profile->commentPrefix.c_str(), pBuf);

        absoluteAddress = oldAbsoluteAddress;
        systemAddress = oldSystemAddress;
    }


    // Check if there is enough room in the last buffer allocated.
    if (!pBuf->CanAddLine(szLine)) {
        // Allocate a new buffer
        pBuf = section->AllocBuffer();
    }

    AddLineInBuffer(disLine, szLine, pBuf);
}

void Disassembly::AddLineWriter(DisassemblySectionType disassemblySectionType) {
    AddLine(lineWriter.GetLineBuffer(), disassemblySectionType);
}

/*
** Add a label definition into buffer.
*/
void Disassembly::AddLabelWithAddress(wstring_view label, Memory::address wAddr, DisassemblySectionType disassemblySectionType, Memory::address wSystemAddr, wstring_view comment) {
    // Build line.
    lineWriter.Clear().String(label).AlignInstructions().String(profile->directiveEQU).Space().Address(wAddr);

    if (!comment.empty()) {
        lineWriter.Comment(comment);
    }

    // Add line to the label buffer.
    systemAddress = disassemblySectionType == DisassemblySectionType::SYSTEM_EQUATES ? wSystemAddr : 0;

    AddLineWriter(disassemblySectionType);
}

void Disassembly::AddLabel(wstring_view label, Memory::address wAddr, DisassemblySectionType disassemblySectionType, wstring_view comment) {
    AddLabelWithAddress(label, wAddr, disassemblySectionType, wAddr, comment);
}

/*
** Add a label which is defined as a value in SYSTEM_EQUATES file.
*/
void Disassembly::AddLabelValue(wstring_view label, Memory::address wAddr, DisassemblySectionType disassemblySectionType, wstring_view comment) {
    const Memory::address wSystemAddr = (disassemblySectionType == DisassemblySectionType::SYSTEM_EQUATES ? 0xFFFF : 0);
    AddLabelWithAddress(label, wAddr, disassemblySectionType, wSystemAddr, comment);
}

/*
** Add a comment line to a buffer.
*/
void Disassembly::AddComment(wstring_view comment, DisassemblySectionType disassemblySectionType) {
    // Build line.
    lineWriter.Clear().Comment(comment);

    // Add line to the label buffer.
    systemAddress = disassemblySectionType == DisassemblySectionType::SYSTEM_EQUATES ? 0xFFFF : 0;
    AddLineWriter(disassemblySectionType);
}

void Disassembly::GenerateUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size) {
    const auto& segment = workspace->GetConstSegment(segmentIndex);

    const auto endOffset = offset + size;
    while (offset < endOffset) {
        auto comment = segment->FindComment(offset);
        if (!comment.empty()) {
            AddUserComment(comment, DisassemblySectionType::CODE_LINES);
        }

        offset++;
    }
}
/*
** Add a user comment into buffer. The comment is split into several lines if needed.
*/
void Disassembly::AddUserComment(wstring_view comment, DisassemblySectionType disassemblySectionType) {
    const auto oldMarkNextSegmentIndex = markNextSegmentIndex;
    const auto oldMarkSegmentIndex = markSegmentIndex;
    const Memory::word wOldMarkOffset = markOffset;
    const Memory::word wOldMarkSize = markSize;

    const auto commentSize = comment.size();
    auto commentBufferSize = commentSize + 1;
    auto szMutableComment = new wchar_t[commentBufferSize];
    wcsncpy(szMutableComment, comment.data(), commentBufferSize);
    szMutableComment[commentBufferSize - 1] = 0;

    auto lpStart = szMutableComment;
    auto lpFullText = new wchar_t[commentSize + profile->commentPrefix.size() + 10]; // TODO: Why +10?

    while (lpStart) {
        bool bReturn = false;
        auto lpEnd = wcschr(lpStart, 0x0A);

        if (lpEnd) {
            *lpEnd = 0;
            if ((lpEnd > lpStart) && (*(lpEnd - 1) == 0x0D)) {
                *(lpEnd - 1) = 0;
                bReturn = true;
            }
        }

        wsprintf(lpFullText, L"%s %s", profile->commentPrefix.c_str(), lpStart);
        markSize = 0;
        systemAddress = 0;
        AddLine(lpFullText, disassemblySectionType);

        if (lpEnd) {
            *lpEnd = 0x0A;
            if (bReturn) {
                *(lpEnd - 1) = 0x0D;
            }

            lpStart = lpEnd + 1;
        }
        else {
            lpStart = nullptr;
        }
    }

    delete[] lpFullText;
    delete[] szMutableComment;

    markNextSegmentIndex = oldMarkNextSegmentIndex;
    markSegmentIndex = oldMarkSegmentIndex;
    markOffset = wOldMarkOffset;
    markSize = wOldMarkSize;
}

/*
** Save the position of a new segment in the disassembly listing.
** Line 1 corresponds to the first line of code.
*/
void Disassembly::SetSegmentFirstLineNumber(SEGMENT_INDEX segmentIndex) {

    // Add this buffer to the end of the list.
    DisassemblyResult::LineCount lineCount = 0;
    if (auto section = result->GetSection(DisassemblySectionType::CODE_LINES)) {
        lineCount += section->GetLineCount();
    }

    workspace->GetSegmentList()->GetSegment(segmentIndex)->SetFirstLineNumber(lineCount + 1);
}

/*
** Adjust the position of all the segments to the abolute line number in the disassembly listing.
** SetSegmentFirstLineNumber has saved a line number relative to start of code.
** We have to add to every segment position the number of lines used by labels.
*/
void Disassembly::AdjustSegmentFirstLineNumber() {
    const auto sectionTypes = { DisassemblySectionType::SYSTEM_EQUATES , DisassemblySectionType::USER_EQUATES , DisassemblySectionType::CODE_EQUATES };
    DisassemblyResult::LineCount equatesLineCount = 0;
    for (auto sectionType : sectionTypes) {
        auto section = result->GetSection(sectionType);
        if (section != nullptr) {
            equatesLineCount += section->GetLineCount();
        }
    }

    const auto& segmentList = workspace->GetSegmentList();
    // Plus the equates fore the code section.
    // Plus 4 for the "; Start of code ..." prelude.
    auto offset = equatesLineCount + 3;
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto& segment = segmentList->GetSegment(segmentIndex);

        const auto localLineNumber = segment->GetFirstLineNumber();
        // Plus 3 per segment, for the empty line, ORG and the following comment line.
        //offset += 3;
        const auto globalLineNumber = localLineNumber + offset;
        // Debug::Log((wstringstream() << "SegmentFirstLineNumber=" << segment->GetFirstLineNumber() << " offset=" << offset << " globalLineNumber=" << globalLineNumber).str());
        segment->SetFirstLineNumber(globalLineNumber);
    }
}

/*
** Set the referenced flag in lines for SYSTEM_EQUATES without offset (sta LABEL).
*/
void Disassembly::SetSystemEquateLinesReferencedBySystemAddress() {
    // TODO: Currently the referenced check does not distinuish the type of access
    // Thefore is $80 is referenced, ZP and DL constants are equally referenced.
    const auto anyAccess = (LabelAccess)((byte)LabelAccess::IMMEDIATE | (byte)LabelAccess::READ_WRITE);

    const auto& systemEquateList = workspace->GetSystemEquateList();
    for (auto i = result->CreateLineIterator(DisassemblySectionType::SYSTEM_EQUATES); i->HasNext();) {
        const auto& disLine = i->Next();
        disLine->referenced = systemEquateList->IsEquateAddressReferenced(disLine->systemAddress, anyAccess);
    }
}

/*
** Set the reference flag in lines for SYSTEM_EQUATES with address (sta LABEL+n).
*/
void Disassembly::SetNearestSystemEquateLineReferencedByAddress(Memory::address address) {
    DIS_LINE* nearestLine = nullptr;

    for (auto i = result->CreateLineIterator(DisassemblySectionType::SYSTEM_EQUATES); i->HasNext();) {
        const auto& disLine = i->Next();

        if ((disLine->systemAddress <= address) && ((nearestLine == nullptr) || (nearestLine->systemAddress < disLine->systemAddress))) {
            nearestLine = disLine;
        }
    }

    if (nearestLine) {
        nearestLine->referenced = true;
    }
}


void Disassembly::DebugSection(DisassemblySectionType disassemblySectionType) {
    Debug::Log((wstringstream() << L"DumpSectionToDebug: disassemblySectionType=" << ((int)disassemblySectionType)).str());
    long count = 0;
    long referencedCount = 0;
    for (auto i = result->CreateLineIterator(disassemblySectionType); i->HasNext();) {
        const auto& disLine = i->Next();
        Debug::Log(disLine->ToString());
        if (disLine->referenced) {
            referencedCount++;
        }
        count++;
    }
    Debug::LogValue(L"count", std::to_wstring(count));
    Debug::LogValue(L"referencedCount", std::to_wstring(referencedCount));

}

/*
** Add all symbol definitions in CODE_EQUATES section for SDX binaries.
*/
void Disassembly::GenerateSDXSymbolDefinitions() {
    const auto& segmentList = workspace->GetConstSegmentList();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto& segment = segmentList->GetConstSegment(segmentIndex);
        if (segment->IsHeader(FileHeader::SDX_SYM_REQUIRED)) {
            wstringstream buffer;
            buffer << segment->szSDXSymbol;

            if (profile->alignInstructions) {
                for (int i = 12 - segment->szSDXSymbol.length(); i > 0; i--) {
                    buffer << " ";
                }
            }
            else {
                buffer << " ";
            }

            buffer << (profile->showLowerCaseInstructions ? "smb" : "SMB") << " " << profile->quoteForASCIIStrings << segment->szSDXSymbol << profile->quoteForASCIIStrings;

            AddLine(buffer.str().c_str(), DisassemblySectionType::CODE_EQUATES);
        }
    }
}

/*
** Give the user a chance to click on "Cancel" button.
*/
bool Disassembly::IsCancelled() {
    if (disassemblyProgressMonitor->IsCancelled()) {
        result->Clear();
        return true;
    }

    return false;
}

void Disassembly::CreateMemoryBlockIterator(const Segment& segment) {
    if (memoryBlockIterator != nullptr) {
        ClearMemoryBlockIterator();
    }
    memoryBlockIterator = new MemoryBlockIterator(segment.memoryBlock);
}

void Disassembly::ClearMemoryBlockIterator() {
    if (memoryBlockIterator != nullptr) {
        delete memoryBlockIterator;
        memoryBlockIterator = nullptr;
    }
}

/*
** Initialize global variables to start of code.
*/
bool Disassembly::DisInit(SEGMENT_INDEX& segmentIndex, Memory::address& pc) {
    pc = 0;
    absoluteAddress = 0;
    systemAddress = 0;

    bDisNewSegment = true;
    memoryBlockIterator = nullptr;

    const auto& segmentList = workspace->GetConstSegmentList();

    for (segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto& segment = segmentList->GetConstSegment(segmentIndex);

        if (segment->bBinary && segment->GetSize() > 0) {
            memoryBlockIterator = new MemoryBlockIterator(segment->memoryBlock);
            pc = segment->wBegin;
            bDisNewSegment = true;
            SetSegmentFirstLineNumber(segmentIndex);
            markNextSegmentIndex = segmentIndex;
            disassemblyProgressMonitor->SetSegmentNumber(segmentIndex + 1);
            return true;
        }
    }

    return false;
}

bool Disassembly::IsInstructionWithImmediate(const Workspace& workspace, SEGMENT_INDEX segmentIndex, Memory::offset offset, byte& immediateValue, MemoryType& immediateMemoryType) {
    immediateValue = 0;
    immediateMemoryType = MemoryType::UNKNOWN;

    const gsl::not_null<const SegmentList*> segmentList = workspace.GetConstSegmentList();
    gsl::not_null<const Segment*> opcodeSegment = segmentList->GetConstSegment(segmentIndex);

    // The code below only depends on the segment and the offset.
    if (opcodeSegment->bBinary && !opcodeSegment->IsEmpty()) {
        byte opc = opcodeSegment->memoryBlock.GetDataAt(offset);
        const auto& instructionSet = workspace.GetInstructionSet(opcodeSegment->processorType);

        if (instructionSet->GetInstruction(opc).GetOperandMode() == OperandMode::Immediate) {
            immediateValue = opcodeSegment->memoryBlock.GetDataAt(offset + 1);

            // The type of the immediate opcode before the known byte is set to
            // MemoryType::LOBYTE or MemoryType::HIBYTE.
            // The user input for the unknown byte in the assumed word is stored in the type
            // of the operand.
            immediateMemoryType = opcodeSegment->GetType(offset);
            if (immediateMemoryType != MemoryType::LOBYTE && immediateMemoryType != MemoryType::HIBYTE) {
                immediateMemoryType = opcodeSegment->GetType(offset + 1);
            }
            return true;
        }
    }
    return false;
}

void Disassembly::DisGetNextByte(byte& bByte, MemoryType& memoryType, SEGMENT_INDEX& segmentIndex, Memory::address& pc) {

    bByte = memoryBlockIterator->GetData();
    memoryType = memoryBlockIterator->GetType();
    memoryBlockIterator->Next();
    markSize++;
    opcodeBuffer.SaveLastOpcode(bByte);

    if (disassemblyProgressMonitor->IsVerbose()) {

        disassemblyProgressMonitor->SendInfo(String::Format(L"segmentIndex={0}, wPC={1}, bByte={2}, memoryType={3}", std::to_wstring(segmentIndex), Memory::address_to_hex_string(pc), Memory::byte_to_hex_string(bByte), Memory::byte_to_hex_string((Memory::byte)memoryType)));
    }

    // End of segment reached?
    if (!memoryBlockIterator->HasNext()) {

        const auto& segmentList = workspace->GetConstSegmentList();
        gsl::not_null<const Segment*> segment = segmentList->GetConstSegment(segmentIndex);
        const auto savedSegmentIndex = segmentIndex;
        const auto lastEndAddress = segment->wEnd;
        const auto wLastHeader = segment->GetHeader();

        ClearMemoryBlockIterator();

        do {
            segmentIndex++;
            if (segmentIndex >= segmentList->GetCount()) {
                ClearMemoryBlockIterator();
                segmentIndex = savedSegmentIndex;
                pc++; // TODO: Why?
                break;
            }

            segment = segmentList->GetConstSegment(segmentIndex);
            if (segment->bBinary && !segment->IsEmpty()) {
                disassemblyProgressMonitor->SetSegmentNumber(segmentIndex + 1);

                CreateMemoryBlockIterator(*segment);
                pc = segment->wBegin;

                if ((lastEndAddress + 1 != pc) ||
                    (!segment->IsHeader(wLastHeader)) ||
                    (!segment->IsHeader(FileHeader::ATARI_BINARY) && !segment->IsHeader(FileHeader::SDX_FIXED_BLK))) {
                    bDisNewSegment = true;
                }

                SetSegmentFirstLineNumber(segmentIndex); // TODO: Why is this called here/so often?
                markNextSegmentIndex = segmentIndex;
            }
        } while (memoryBlockIterator == nullptr);
    }
    else {
        pc++;
    }
}

/*
** Returns next byte of code in bByte and increment wPC which is the address of the byte.
** If a segment limit has been crossed (the new segment is not contiguous with the
** previous one), bDisNewSegment is set. The kind of byte is set in cDisByteType.
** pDisNextByte is incremented. At the end of the code, the last byte is returned
** and pDisNextByte is nullptr.
*/
#define DIS_GET_NEXT_BYTE(bByte, segmentIndex, wPC) { \
     DisGetNextByte(bByte, cDisByteType, segmentIndex, wPC); \
     if (IsCancelled()) { \
          return; \
	 } \
}


void Disassembly::AddOrgOrBlock(SEGMENT_INDEX segmentIndex, Memory::address wPC) {
    const auto& segmentList = workspace->GetSegmentList();
    const auto& segment = segmentList->GetConstSegment(segmentIndex);

    switch (segment->GetHeader()) {
    case FileHeader::SDX_FIXED_BLK:
        lineWriter.CString(L"blk sparta").Space().Address(wPC);
        break;

    case FileHeader::SDX_RELOC_BLK: {
        const auto memoryType = segment->GetSDXMemoryType();
        if (segment->IsSDXRelocBlkWithData()) {
            lineWriter.CString(L"blk reloc");
        }
        else {
            lineWriter.CString(L"blk empty").Space().Number(segment->GetSize());
        }
        lineWriter.Space().String(memoryType).Space().String(profile->commentPrefix).CString(L" num: ").ByteNumber(segment->bSDXBlockNumber).CString(L" mem: ").ByteNumber(segment->bSDXControlByte);
        break;
    }

    case FileHeader::SDX_FIX_UP_BLK:
        lineWriter.CString(L"blk update addresses");
        break;

    case FileHeader::SDX_SYM_REQUIRED:
        lineWriter.CString(L"blk update symbols");
        break;

    case FileHeader::SDX_SYM_DEFINED: {
        const auto& equate = workspace->GetUserEquateList()->FindEquateByAddress(segment->wBegin, LabelAccess::READ, true);
        wstring label;
        if (equate == nullptr) {
            const auto& labelSegment = segmentList->FindBySDXBlockNumber(segment->bSDXBlockNumber);
            wsprintf(String::szBuffer, Segment::szDefaultLabelFormat, segmentList->GetSegmentIndex(labelSegment), segment->wBegin);
            label = String::Format();
        }
        else {
            label = equate->GetLabel();
        }
        lineWriter.CString(L"blk update new").Space().String(label).Space().Char('\'').String(segment->szSDXSymbol).Char('\'');
        break;
    }

    default:
        lineWriter.String(profile->directiveORG).Space().Address(wPC);
        break;
    }
}

/*
** Pass 2: reserve all labels.
*/
void Disassembly::Pass2() {


    /*
    ** Returns next byte in cLow for Pass 2.
    */
#define DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC) \
	{ \
	if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
		break; \
	} \
	DIS_GET_NEXT_BYTE(cLow, segmentIndex, wPC); \
}


    /*
    ** Returns next word in wAddr for Pass 2.
    */
#define DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC) { \
     byte cL, cH; \
     if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
          break; \
	 	 	 	      } \
     DIS_GET_NEXT_BYTE(cL, segmentIndex, wPC); \
     if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
          break; \
	 	 	 	      } \
     DIS_GET_NEXT_BYTE(cH, segmentIndex, wPC); \
     wAddr = Memory::to_address(cL,cH); \
}


    Memory::byte bByte;
    MemoryType cDisByteType;

    Memory::byte cLow;
    Memory::address wAddr;
    Memory::address wPC;
    SEGMENT_INDEX segmentIndex;

    const auto& segmentList = workspace->GetSegmentList();

    if (!DisInit(segmentIndex, wPC)) {
        return;
    }

    while (memoryBlockIterator != nullptr) {
        bDisNewSegment = false;
        auto oldSegmentIndex = segmentIndex;
        auto wOldPC = wPC;
        const auto& opcodeSegment = segmentList->GetConstSegment(segmentIndex);
        const auto& instructionSet = workspace->GetInstructionSet(opcodeSegment->processorType);

        DIS_GET_NEXT_BYTE(bByte, segmentIndex, wPC);
        switch (cDisByteType) {

            // No label: this is a byte.
        case MemoryType::BYTE:
        case MemoryType::STRING:
        case MemoryType::SBYTE:
        case MemoryType::STORE:
            break;

            // No label: this is a word.
        case MemoryType::WORD:
        case MemoryType::SYMBOL:
            DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
            break;

            // This is an address fix-up
        case MemoryType::FIXUP:
            DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
            wAddr = Memory::to_address(bByte, cLow);
            segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ);
            break;

            // This is a label
        case MemoryType::LABEL:
            DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
            wAddr = Memory::to_address(bByte, cLow);
            segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ);
            break;

            // There may be a label: this is the display list.
        case MemoryType::DLIST:
            switch (bByte & 0x0F) {

                // Empty lines
            case 0:
                break;

                // Jump
            case 1:
                oldSegmentIndex = segmentIndex;
                wOldPC = wPC;
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ);
                break;

                // Graphic lines
            default:
                // Load Memory Scan
                if (bByte & 0x40) {
                    oldSegmentIndex = segmentIndex;
                    wOldPC = wPC;
                    DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                    segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ);
                }
                break;
            }
            break;

            // Code
        case MemoryType::LOBYTE:
        case MemoryType::HIBYTE:
        case MemoryType::UNKNOWN:
        case MemoryType::CODE:
            if ((instructionSet->GetInstruction(bByte).IsUnsupportedInstruction()) && (profile->useIllegalOpcodes == false)) {
                break;
            }

            switch (instructionSet->GetInstruction(bByte).GetOperandMode()) {

                // Accumulator mode has no additional operands.
            case OperandMode::Accumulator:
                break;

                // Get absolute address.
            case OperandMode::Absolute:
            case OperandMode::Indirect:
            case OperandMode::AbsoluteX:
            case OperandMode::AbsoluteY:
            case OperandMode::IndexedIndirectAbsolute:
                oldSegmentIndex = segmentIndex;
                wOldPC = wPC;
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, instructionSet->GetInstruction(bByte));
                break;

                // Get zero-page absolute address.
            case OperandMode::ZeroPageX:
            case OperandMode::ZeroPageY:
            case OperandMode::ZeroPage:
            case OperandMode::IndexedIndirect:
            case OperandMode::IndirectIndexed:
            case OperandMode::ZeroPageIndirect:
            case OperandMode::ZeroPageRelative:
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                segmentList->AllocateAddress(oldSegmentIndex, wOldPC, cLow, cDisByteType, instructionSet->GetInstruction(bByte));
                break;

                // Get relative address and make an absolute one.
            case OperandMode::Relative:
                wOldPC = wPC;
                oldSegmentIndex = segmentIndex;
                wAddr = wPC + 1;
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                if (cLow > 127) {
                    wAddr += cLow - 256;
                }
                else {
                    wAddr += cLow;
                }
                segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, instructionSet->GetInstruction(bByte));
                break;

                // Other modes with a parameter that is not an address.
            case OperandMode::Immediate:
                if (cDisByteType == MemoryType::LOBYTE) {
                    DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                    wAddr = Memory::to_address(cLow, (Memory::byte)cDisByteType);
                    segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, instructionSet->GetInstruction(bByte));
                }
                else if (cDisByteType == MemoryType::HIBYTE) {
                    DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                    wAddr = Memory::to_address((Memory::byte)cDisByteType, cLow);
                    segmentList->AllocateAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, instructionSet->GetInstruction(bByte));
                }
                else {
                    DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                }
                break;

                // Implied mode has no additional operands.
            case OperandMode::Implied:
                break;

                // Reserved 65C02 NOP (1 byte) has no additional operands.
            case OperandMode::ReservedNop1Byte:
                break;

                // Reserved 65C02 NOP (2 bytes) has one additional operand.
            case OperandMode::ReservedNop2Byte:
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC); // Unused byte
                break;

                // Reserved 65C02 NOP (3 bytes) has two additional operands.
            case OperandMode::ReservedNop3Byte:
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC); // Unused byte
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC); // Unused byte
                break;
            }
            break;

        }
    }
}

/*
** Pass 3: update labels to generate relative labels (L2222+1 instead of L2223 if L2223 is inside an instruction).
*/
void Disassembly::Pass3() {

    /*
    ** Returns next byte in cLow for Pass 2.
    */
#define DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC) \
	{ \
	if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
		break; \
	} \
	DIS_GET_NEXT_BYTE(cLow, segmentIndex, wPC); \
}

    byte bByte;
    MemoryType cDisByteType;

    Memory::byte cLow;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-but-set-variable"
    Memory::address wAddr = 0; // Used as parameter in macro calls
#pragma clang diagnostic pop
    Memory::address wPC;
    SEGMENT_INDEX segmentIndex = 0;

    const auto& segmentList = workspace->GetSegmentList();

    // determine which label address is at the start of an instruction (aligned)
    if (!DisInit(segmentIndex, wPC)) {
        return;
    }

    while (memoryBlockIterator != nullptr) {
        const auto& lpOpcodeSegment = segmentList->GetSegment(segmentIndex);
        const auto& instructionSet = workspace->GetInstructionSet(lpOpcodeSegment->processorType);
        bDisNewSegment = false;
        lpOpcodeSegment->DefineAddressLabel(wPC);
        DIS_GET_NEXT_BYTE(bByte, segmentIndex, wPC);

        switch (cDisByteType) {

            // no label: this is a byte.
        case MemoryType::BYTE:
        case MemoryType::STRING:
        case MemoryType::SBYTE:
        case MemoryType::STORE:
            break;

            // no label: this is a word.
        case MemoryType::WORD:
        case MemoryType::FIXUP:
        case MemoryType::SYMBOL:
        case MemoryType::LABEL:
            DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
            break;

            // there may be a label: this is the display list.
        case MemoryType::DLIST:
            switch (bByte & 0x0F) {
            case 0:
                break;

            case 1:
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                break;

            default:
                if (bByte & 0x40) {
                    DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                }
                break;
            }
            break;

            // code.
        case MemoryType::LOBYTE:
        case MemoryType::HIBYTE:
        case MemoryType::UNKNOWN:
        case MemoryType::CODE:
            if ((instructionSet->GetInstruction(bByte).IsUnsupportedInstruction()) && (profile->useIllegalOpcodes == false)) {
                break;
            }

            switch (instructionSet->GetInstruction(bByte).GetOperandMode()) {

                // Modes without additional operands.
            case OperandMode::Accumulator:
            case OperandMode::Implied:
            case OperandMode::ReservedNop1Byte:
                break;

                // Get absolute address.
            case OperandMode::Absolute:
            case OperandMode::Indirect:
            case OperandMode::AbsoluteX:
            case OperandMode::AbsoluteY:
            case OperandMode::IndexedIndirectAbsolute:
            case OperandMode::ReservedNop3Byte:
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                break;

                // Get zero-page absolute address.
            case OperandMode::ZeroPageX:
            case OperandMode::ZeroPageY:
            case OperandMode::ZeroPage:
            case OperandMode::IndexedIndirect:
            case OperandMode::IndirectIndexed:
            case OperandMode::Relative:
            case OperandMode::Immediate:
            case OperandMode::ZeroPageIndirect:
            case OperandMode::ZeroPageRelative:
            case OperandMode::ReservedNop2Byte:
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                break;
            }
            break;
        }
    }

    // for those non-aligned label addresses, find the immediate previous one
    segmentList->AlignRamBlkLabelAddresses();
    if (!DisInit(segmentIndex, wPC)) {
        return;
    }

    while (memoryBlockIterator != nullptr) {
        const auto& opcodeSegment = segmentList->GetSegment(segmentIndex);
        const auto& instructionSet = workspace->GetInstructionSet(opcodeSegment->processorType);

        bDisNewSegment = false;
        opcodeSegment->AlignAddressLabels(wPC);
        DIS_GET_NEXT_BYTE(bByte, segmentIndex, wPC);

        switch (cDisByteType) {

            // no label: this is a byte.
        case MemoryType::BYTE:
        case MemoryType::STRING:
        case MemoryType::SBYTE:
        case MemoryType::STORE:
            break;

            // no label: this is a word.
        case MemoryType::WORD:
        case MemoryType::FIXUP:
        case MemoryType::SYMBOL:
        case MemoryType::LABEL:
            DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
            break;

            // there may be a label: this is the display list.
        case MemoryType::DLIST:
            switch (bByte & 0x0F) {
            case 0:
                break;

            case 1:
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                break;

            default:
                if (bByte & 0x40) {
                    DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                }
                break;
            }
            break;

            // code.
        case MemoryType::LOBYTE:
        case MemoryType::HIBYTE:
        case MemoryType::UNKNOWN:
        case MemoryType::CODE:
            if ((instructionSet->GetInstruction(bByte).IsUnsupportedInstruction()) && (profile->useIllegalOpcodes == false)) {
                break;
            }

            switch (instructionSet->GetInstruction(bByte).GetOperandMode()) {

                // Modes without additional operands.
            case OperandMode::Accumulator:
            case OperandMode::Implied:
            case OperandMode::ReservedNop1Byte:
                break;

                // Get absolute address.
            case OperandMode::Absolute:
            case OperandMode::Indirect:
            case OperandMode::AbsoluteX:
            case OperandMode::AbsoluteY:
            case OperandMode::IndexedIndirectAbsolute:
            case OperandMode::ReservedNop3Byte:
                DIS_GET_WORD_IN_PASS_2(wAddr, segmentIndex, wPC);
                break;

                // Get zero-page absolute address.
            case OperandMode::ZeroPageX:
            case OperandMode::ZeroPageY:
            case OperandMode::ZeroPage:
            case OperandMode::IndexedIndirect:
            case OperandMode::IndirectIndexed:
            case OperandMode::Relative:
            case OperandMode::Immediate:
            case OperandMode::ZeroPageIndirect:
            case OperandMode::ZeroPageRelative:
            case OperandMode::ReservedNop2Byte:
                DIS_GET_BYTE_IN_PASS_2(cLow, segmentIndex, wPC);
                break;
            }
            break;
        }
    }
}


/*
** Pass 4: Generate listing
*/
void Disassembly::Pass4() {


    /*
    ** Returns next byte in cLow for Pass 4.
    */
#define DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC) { \
     if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
          disassemblyWriter.DUMP_BYTE( bByte); \
          break; \
	 } \
     DIS_GET_NEXT_BYTE(cLow, segmentIndex, wPC); \
}

    /*
    ** returns next word in wAddr for Pass 4.
    */
#define DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC) { \
     byte cL, cH; \
     if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
          disassemblyWriter.DUMP_BYTE( bByte); \
          break; \
	 } \
     DIS_GET_NEXT_BYTE(cL, segmentIndex, wPC); \
     if (bDisNewSegment || (memoryBlockIterator == nullptr)) { \
          disassemblyWriter.DUMP_BYTE( bByte); \
          disassemblyWriter.DUMP_BYTE( cL); \
          break; \
	 	 	 	      } \
     DIS_GET_NEXT_BYTE(cH, segmentIndex, wPC); \
     wAddr = ((WORD) cL) | (((WORD) cH) << 8); \
}

    Memory::byte bByte;
    MemoryType cDisByteType;

    Memory::byte cLow = 0;
    Memory::address wAddr = 0;
    Memory::address wOldPC = 0;
    Memory::address wPC = 0;
    SEGMENT_INDEX segmentIndex = 0;
    bool bAnticLabel = false;

    const auto& segmentList = workspace->GetSegmentList();
    DisassemblyWriter disassemblyWriter(*this, *workspace);

    markSegmentIndex = NO_SEGMENT_INDEX;
    markNextSegmentIndex = NO_SEGMENT_INDEX;

    if (!DisInit(segmentIndex, wPC)) {
        return;
    }

    bAnticLabel = false;
    while (memoryBlockIterator != nullptr) {
        if (bDisNewSegment) {
            bDisNewSegment = false;
            lineWriter.Clear();
            disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, 0);

            lineWriter.Clear().AlignInstructions();
            AddOrgOrBlock(segmentIndex, wPC);
            AddLineWriter();
            AddEmptyCommentLine();
        }

        lineWriter.Clear();
        bool newLabelDefined;
        auto newLabel = segmentList->DefineLabelAtAddress(segmentIndex, wPC, newLabelDefined);

        if (newLabelDefined && !Equate::IsLabelWithOffset(newLabel)) {
            disassemblyWriter.FLUSH_BYTES();
            lineWriter.String(newLabel);

            if (profile->showColonAfterLabel) {
                lineWriter.Char(':');
            }
        }
        lineWriter.AlignInstructions();

        auto oldSegmentIndex = segmentIndex;
        wOldPC = wPC;
        const auto& opcodeSegment = segmentList->GetConstSegment(segmentIndex);
        const auto& instructionSet = workspace->GetInstructionSet(opcodeSegment->processorType);

        DIS_GET_NEXT_BYTE(bByte, segmentIndex, wPC);
        GenerateUserComment(oldSegmentIndex, wOldPC - workspace->GetConstSegment(segmentIndex)->wBegin, 1); // TODO: Why was this GetOpcodeLength(bByte));

        if (cDisByteType != MemoryType::DLIST) {
            bAnticLabel = false;
        }

        switch (cDisByteType) {

            // N bytes skipped in memory (data storage)
        case MemoryType::STORE:
            if (profile->directiveDSAllowed) {
                disassemblyWriter.DUMP_STORE();
            }
            else {
                disassemblyWriter.DUMP_BYTE(bByte);
            }
            break;

            // one byte in hexadecimal format
        case MemoryType::BYTE:
            disassemblyWriter.DUMP_BYTE(bByte);
            break;

            // one byte in string format
        case MemoryType::STRING:
            disassemblyWriter.DUMP_STRING(bByte);
            break;

            // one byte in internal format
        case MemoryType::SBYTE:
            if (profile->directiveSBYTEAllowed) {
                disassemblyWriter.DUMP_SBYTE(bByte);
            }
            else {
                disassemblyWriter.DUMP_BYTE(bByte);
            }
            break;

            // two bytes in a WORD (MSB first)
        case MemoryType::WORD:
            DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
            if (profile->directiveWORDAllowed) {
                wAddr = ((WORD)bByte) | (((WORD)cLow) << 8);
                disassemblyWriter.DUMP_WORD(wAddr);
            }
            else {
                disassemblyWriter.DUMP_BYTE(bByte);
                disassemblyWriter.DUMP_BYTE(cLow);
            }
            break;

            // an SDX system label (absolute address)
        case MemoryType::SYMBOL:
        {
            DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
            wAddr = Memory::to_address(bByte, cLow);
            const wchar_t* szLabel;
            if ((profile->directiveBYTEOnlyNumbersAllowed == FALSE) && (szLabel = workspace->FindSymbolByAddress(oldSegmentIndex, wOldPC, wAddr))) {
                disassemblyWriter.DUMP_LABEL(szLabel);
            }
            else {
                disassemblyWriter.DUMP_WORD(wAddr);
            }
            break;
        }

        // an address fix-up (absolute address)
        case MemoryType::FIXUP:
        {
            DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
            wAddr = ((WORD)bByte) | (((WORD)cLow) << 8);
            auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ);
            if (!label.empty()) {
                disassemblyWriter.DUMP_LABEL(label.c_str());
            }
            else {
                if (profile->directiveWORDAllowed) {
                    disassemblyWriter.DUMP_WORD(wAddr);
                }
                else {
                    disassemblyWriter.DUMP_BYTE((byte)((wAddr >> 8) & 0xFF));
                    disassemblyWriter.DUMP_BYTE((byte)(cLow & 0xFF));
                }
            }
        }
        break;

        // a label (absolute address)
        case MemoryType::LABEL:
            DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
            if (profile->directiveWORDAllowed) {
                wAddr = ((WORD)bByte) | (((WORD)cLow) << 8);
                auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ_WRITE);
                if ((profile->directiveBYTEOnlyNumbersAllowed == FALSE) && !label.empty()) {
                    disassemblyWriter.DUMP_LABEL(label.c_str());
                }
                else {
                    disassemblyWriter.DUMP_WORD(wAddr);
                }
            }
            else {
                disassemblyWriter.DUMP_BYTE(bByte);
                disassemblyWriter.DUMP_BYTE(cLow);
            }
            break;

            // this is the display list.
        case MemoryType::DLIST:
            if (profile->directiveBYTEOnlyNumbersAllowed) {
                disassemblyWriter.DUMP_BYTE(bByte);
            }
            else {
                if (bAnticLabel) {
                    DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                    if (profile->directiveWORDAllowed) {
                        wAddr = Memory::to_address(bByte, cLow);
                        auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, OperandMode::Accumulator, LabelAccess::READ_WRITE);
                        if (!label.empty()) {
                            disassemblyWriter.DUMP_LABEL(label.c_str());
                        }
                        else {
                            disassemblyWriter.DUMP_WORD(wAddr);
                        }
                    }
                    else {
                        disassemblyWriter.DUMP_BYTE(bByte);
                        disassemblyWriter.DUMP_BYTE(cLow);
                    }

                    bAnticLabel = false;
                }
                else switch (bByte & 0x0F) {

                    // empty lines
                case 0:
                    lineWriter.String(profile->directiveBYTE).Space();
                    if ((bByte & 0x80)) {
                        lineWriter.CString(L"ADLI+");
                    }
                    lineWriter.CString(L"AEMPTY").Decimal(((bByte & 0x70) >> 4) + 1);
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, 0);
                    break;

                    // jump
                case 1:
                    lineWriter.String(profile->directiveBYTE).Space();
                    if ((bByte & 0x80)) {
                        lineWriter.CString(L"ADLI+");
                    }
                    if ((bByte & 0x40)) {
                        lineWriter.CString(L"AVB+");
                    }
                    lineWriter.CString(L"AJMP");
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, 0);
                    bAnticLabel = true;
                    break;

                    // graphic lines
                default:
                    // Load Memory Scan
                    if (bByte & 0xF0) {
                        lineWriter.String(profile->directiveBYTE).Space();
                        if (bByte & 0x80) {
                            lineWriter.CString(L"ADLI+");
                        }
                        if (bByte & 0x20) {
                            lineWriter.CString(L"AVSCR+");
                        }
                        if (bByte & 0x10) {
                            lineWriter.CString(L"AHSCR+");
                        }
                        if (bByte & 0x40) {
                            lineWriter.CString(L"ALMS+");
                            bAnticLabel = true;
                        }
                        lineWriter.Number(bByte & 0x0F);
                        disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, 0);
                    }
                    else {
                        disassemblyWriter.DUMP_BYTE(bByte);
                    }
                    break;
                }
            }
            break;

            // code.
        case MemoryType::LOBYTE:
        case MemoryType::HIBYTE:
        case MemoryType::UNKNOWN:
        case MemoryType::CODE:
            if ((instructionSet->GetInstruction(bByte).IsUnsupportedInstruction()) && (profile->useIllegalOpcodes == false)) {
                disassemblyWriter.DUMP_BYTE(bByte);
                break;
            }

            switch (instructionSet->GetInstruction(bByte).GetOperandMode()) {
            case OperandMode::Immediate: {
                if (cDisByteType == MemoryType::LOBYTE) {
                    DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                    wAddr = ((WORD)cLow) | (((WORD)cDisByteType) << 8);

                    lineWriter.Instruction(opcodeSegment, bByte).CString(L" #");
                    auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    if (!label.empty()) {
                        lineWriter.String(profile->directiveLOWHead).Label(label).String(profile->directiveLOWTail);
                    }
                    else {
                        lineWriter.Byte(cLow);
                    }
                }
                else if (cDisByteType == MemoryType::HIBYTE) {
                    DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                    wAddr = ((word)cDisByteType) | (((word)cLow) << 8);

                    lineWriter.Instruction(opcodeSegment, bByte).CString(L" #");
                    auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    if (!label.empty()) {
                        lineWriter.String(profile->directiveHIGHHead).Label(label).String(profile->directiveHIGHTail);
                    }
                    else {
                        lineWriter.Byte(cLow);
                    }
                }
                else {
                    DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                    lineWriter.Instruction(opcodeSegment, bByte).CString(L" #");
                    if (cDisByteType == MemoryType::STRING && disassemblyWriter.IsByteAllowedInString(cLow)) {
                        lineWriter.String(profile->quoteForASCIIStrings).Char(cLow).String(profile->quoteForASCIIStrings);
                    }
                    else {
                        lineWriter.Byte(cLow);
                    }
                }
                // TODO: Remove when stable, String::AssertASCII(lineWriter.GetLineBuffer());

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, 0);
                break;
            }

            case OperandMode::Absolute: {
                wOldPC = wPC;
                oldSegmentIndex = segmentIndex;
                DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC);
                const wchar_t* szLabel;
                if ((cDisByteType == MemoryType::SYMBOL) && (szLabel = workspace->FindSymbolByAddress(oldSegmentIndex, wOldPC, wAddr))) {
                    lineWriter.Instruction(opcodeSegment, bByte).Space().Label(szLabel);
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                    break;
                }
                if ((wAddr < 0x100) && (profile->showZPAbsoluteAsByte) && (bByte != 0x20 /*JSR*/) && (bByte != 0x4C /*JMP*/)) {
                    disassemblyWriter.FLUSH_BYTES();
                    lineWriter.String(profile->directiveBYTE).Space().Number(bByte).String(profile->directiveBYTESeparator).Number(wAddr & 0xFF).String(profile->directiveBYTESeparator).Number((wAddr >> 8) & 0xFF);
                    lineWriter.SpaceUntil34();
                    lineWriter.Space().String(profile->commentPrefix).Space().Instruction(opcodeSegment, bByte).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);

                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                }
                else {
                    lineWriter.Instruction(opcodeSegment, bByte, wAddr).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);

                    disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, wAddr);
                }
                break;
            }

            case OperandMode::ZeroPage: {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                lineWriter.Instruction(opcodeSegment, bByte).Space();
                auto addr = Memory::to_address(cLow, 0);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }
            case OperandMode::Accumulator:
            {
                lineWriter.Instruction(opcodeSegment, bByte);
                if (profile->showAInAccumulatorMode) {
                    lineWriter.CString(L" A");
                }
                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 1, 0);
                break;
            }

            case OperandMode::Implied:
            {
                if ((bByte == 0) && (profile->showBRKAsByte0)) {
                    disassemblyWriter.DUMP_BYTE(bByte);
                }
                else {
                    lineWriter.Instruction(opcodeSegment, bByte);
                    disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 1, 0);
                }
                break;
            }

            case OperandMode::IndexedIndirect:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).CString(L" (");
                auto addr = Memory::to_address(cLow, 0);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L",X)");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::IndirectIndexed:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).CString(L" (");
                auto addr = Memory::to_address(cLow, 0);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L"),Y");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::ZeroPageX:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).Space();
                auto addr = Memory::to_address(cLow, 0);
                auto label = segmentList->GetLabelAtAddress(oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L",X");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::ZeroPageY:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).Space();
                auto addr = Memory::to_address(cLow, 0);

                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L",Y");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::AbsoluteX: {
                wOldPC = wPC;
                oldSegmentIndex = segmentIndex;
                DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC);
                const wchar_t* szLabel;
                if ((cDisByteType == MemoryType::SYMBOL) && (szLabel = workspace->FindSymbolByAddress(segmentIndex, wPC - 2, wAddr))) {
                    lineWriter.Instruction(opcodeSegment, bByte).Space().Label(szLabel).CString(L",X");
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                    break;
                }

                if ((wAddr < 0x100) && (profile->showZPAbsoluteAsByte) && (bByte != 0x20 /*JSR*/) && (bByte != 0x4C /*JMP*/)) {
                    disassemblyWriter.FLUSH_BYTES();
                    lineWriter.String(profile->directiveBYTE).Space();
                    lineWriter.Byte(bByte).String(profile->directiveBYTESeparator);
                    lineWriter.Byte(wAddr & 0xFF).String(profile->directiveBYTESeparator);
                    lineWriter.Byte((wAddr >> 8) & 0xFF);
                    lineWriter.SpaceUntil34();
                    lineWriter.Space().String(profile->commentPrefix).Space().Instruction(opcodeSegment, bByte).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    lineWriter.CString(L",X");

                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                }
                else {
                    lineWriter.Instruction(opcodeSegment, bByte, wAddr).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    lineWriter.CString(L",X");
                    disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, wAddr);
                }
                break;
            }

            case OperandMode::AbsoluteY:
            {
                wOldPC = wPC;
                oldSegmentIndex = segmentIndex;
                DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC);

                const wchar_t* szLabel;
                if ((cDisByteType == MemoryType::SYMBOL) && (szLabel = workspace->FindSymbolByAddress(segmentIndex, wPC - 2, wAddr))) {
                    lineWriter.Instruction(opcodeSegment, bByte).Space().Label(szLabel).CString(L",Y");
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                    break;
                }

                if ((wAddr < 0x100) && (profile->showZPAbsoluteAsByte) && (bByte != 0x20 /*JSR*/) && (bByte != 0x4C /*JMP*/)) {
                    disassemblyWriter.FLUSH_BYTES();
                    lineWriter.String(profile->directiveBYTE).Space();
                    lineWriter.Byte(bByte).String(profile->directiveBYTESeparator);
                    lineWriter.Byte(wAddr & 0xFF).String(profile->directiveBYTESeparator);
                    lineWriter.Byte((wAddr >> 8) & 0xFF);
                    lineWriter.SpaceUntil34();
                    lineWriter.Space().String(profile->commentPrefix).Space().Instruction(opcodeSegment, bByte).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    lineWriter.CString(L",Y");
                    disassemblyWriter.FLUSH_AND_ADD_LINE(lineWriter, wAddr);
                }
                else {
                    lineWriter.Instruction(opcodeSegment, bByte, wAddr).Space();
                    lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                    lineWriter.CString(L",Y");

                    disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, wAddr);
                }
                break;
            }

            case OperandMode::Relative:
            {
                wOldPC = wPC;
                oldSegmentIndex = segmentIndex;
                wAddr = wPC + 1;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                if (cLow > 127) {
                    wAddr += (Memory::word)cLow - 256;
                }
                else {
                    wAddr += (Memory::word)cLow;
                }

                lineWriter.Instruction(opcodeSegment, bByte).Space();
                lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, wAddr);
                break;
            }

            case OperandMode::Indirect:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).CString(L" (");
                lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                lineWriter.CString(L")");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, wAddr);
                break;
            }

            case OperandMode::ZeroPageIndirect:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).CString(L" (");
                auto addr = Memory::to_address(cLow, 0);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L")");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::ZeroPageRelative:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).Space();
                const auto addr = Memory::to_address(cLow, 0);
                lineWriter.LabelOrZeroPageAddress(*segmentList, oldSegmentIndex, wOldPC, addr, cDisByteType, bByte);
                lineWriter.CString(L",X");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, addr);
                break;
            }

            case OperandMode::IndexedIndirectAbsolute:
            {
                oldSegmentIndex = segmentIndex;
                DIS_GET_WORD_IN_PASS_4(bByte, wAddr, segmentIndex, wPC);

                lineWriter.Instruction(opcodeSegment, bByte).CString(L" (");
                lineWriter.LabelOrAddress(*segmentList, oldSegmentIndex, wOldPC, wAddr, cDisByteType, bByte);
                lineWriter.CString(L",X)");

                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, wAddr);
                break;
            }

            case OperandMode::ReservedNop1Byte:
            {
                lineWriter.String(profile->directiveBYTE).Space();
                lineWriter.Byte(bByte).Space();
                lineWriter.String(profile->commentPrefix).CString(L" NOP");
                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 1, 0);
                break;
            }

            case OperandMode::ReservedNop2Byte:
            {
                DIS_GET_BYTE_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                lineWriter.String(profile->directiveBYTE).Space();
                lineWriter.Byte(bByte).String(profile->directiveBYTESeparator);
                lineWriter.Byte(wAddr & 0xFF).Space();
                lineWriter.String(profile->commentPrefix).CString(L" NOP");
                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 2, 0);
                break;
            }

            case OperandMode::ReservedNop3Byte:
            {
                DIS_GET_WORD_IN_PASS_4(bByte, cLow, segmentIndex, wPC);
                lineWriter.String(profile->directiveBYTE).Space();
                lineWriter.Byte(bByte).String(profile->directiveBYTESeparator);
                lineWriter.Byte(wAddr & 0xFF).String(profile->directiveBYTESeparator);
                lineWriter.Byte((wAddr >> 8) & 0xFF).Space();
                lineWriter.String(profile->commentPrefix).CString(L" NOP");
                disassemblyWriter.FLUSH_AND_ADD_LINE_WITH_COMMENT(lineWriter, opcodeBuffer, 3, 0);
                break;
            }
            }
            break;

            // default is an error.
        default:
            disassemblyWriter.DUMP_BYTE(bByte);
            break;
        }
    }

    disassemblyWriter.FLUSH_BYTES();
}

/*
** Pass 5: generate SDX special directive for symbols or fix-ups.
*/
void Disassembly::Pass5() {
    const auto& lpSegmentList = workspace->GetSegmentList();

    auto bFixups = false;
    auto bSymReq = false;
    auto wLastHeader = FileHeader::RAW;

    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < lpSegmentList->GetCount(); segmentIndex++) {
        const auto& segment = lpSegmentList->GetConstSegment(segmentIndex);
        markSegmentIndex = segmentIndex;
        markOffset = markSize = 0;

        lineWriter.Clear().AlignInstructions();

        switch (segment->GetHeader()) {
        case FileHeader::SDX_FIX_UP_BLK:
            if (!bFixups) {
                bFixups = true;
                AddEmptyCommentLine();
                AddOrgOrBlock(segmentIndex, 0);
                AddLineWriter();
            }
            break;

        case FileHeader::SDX_SYM_REQUIRED:
            if (!bSymReq) {
                bSymReq = true;
                AddEmptyCommentLine();
                AddOrgOrBlock(segmentIndex, 0);
                AddLineWriter();
            }
            break;

        case FileHeader::SDX_RELOC_BLK:
            if (segment->IsSDXRelocBlkWithoutData()) {
                AddEmptyCommentLine();
                AddOrgOrBlock(segmentIndex, 0);
                AddLineWriter();
                AddEmptyCommentLine();
                wsprintf(String::szBuffer, L"S%03hXEND", segmentIndex);
                auto endLabel = String::Format();
                lineWriter.Clear().String(endLabel);
                if (profile->showColonAfterLabel) {
                    lineWriter.Char(':');
                }
                AddLineWriter();

                for (Memory::word wIndex = segment->wBegin; wIndex <= segment->wEnd; wIndex++) {
                    bool labelDefined;
                    auto label = lpSegmentList->DefineLabelAtAddress(segmentIndex, wIndex, labelDefined);

                    if (labelDefined && !Equate::IsLabelWithOffset(label)) {
                        lineWriter.Clear().String(label).AlignInstructions();
                        lineWriter.String(profile->directiveEQU).Space().String(endLabel).Char('-').Number(segment->wEnd - wIndex + 1);
                        AddLineWriter();
                    }
                }
            }
            break;

        case FileHeader::SDX_SYM_DEFINED:
            if (!segment->IsHeader(wLastHeader)) {
                AddEmptyCommentLine();
            }

            AddOrgOrBlock(segmentIndex, 0);
            AddLineWriter();
            break;

        default:
            break;
        }

        wLastHeader = segment->GetHeader();
    }
}

void Disassembly::SetPass(int pass, wstring_view text) {
    this->pass = pass;
    disassemblyProgressMonitor->SetPass(String::Format(L"{0} - {1}", std::to_wstring(pass), text));
}

void Disassembly::DisassembleInternal() {
    this->pass = 0;
    workspace->GetDisassemblyResult()->Clear();
    workspace->ClearEquateFlags();
    const auto& segmentList = workspace->GetSegmentList();
    segmentList->FreeAllSymbols();
    segmentList->FreeAllFixups();
    segmentList->FreeAllFixupAddressLabels();
    segmentList->FreeAllAddresses();

    opcodeBuffer.Clear();

    lineWriter.Init(*workspace);

    // Pass 1: Find all labels.
    SetPass(1, L"Find Labels");
    Pass1::Execute(*workspace);

    // Pass 2: Reserve all labels.
    SetPass(2, L"Reserve Labels");
    Pass2();

    // Pass 3: Update labels to generate relative labels.
    SetPass(3, L"Update Labels");
    Pass3();

    // Pass 4: Generate listing.
    SetPass(4, L"Generate Listing");
    Pass4();

    // Pass 5: Add non-code segments (RamBlk, FixUps, ...).
    SetPass(5, L"Add Non - Code Segments");
    Pass5();

    SetPass(6, L"Cleanup");

    // Mark thebase labels of ranges as referenced
    workspace->GetSystemEquateList()->SetBaseLabelsReferenced();
    workspace->GetUserEquateList()->SetBaseLabelsReferenced();

    // Start creating the result.
    GenerateEquates(DisassemblySectionType::SYSTEM_EQUATES, *workspace->GetSystemEquateList());
    GenerateEquates(DisassemblySectionType::USER_EQUATES, *workspace->GetUserEquateList());

    // Generate labels that have not been defined in the code listing.
    GenerateCodeEquates();
    GenerateSDXSymbolDefinitions();

    Debug::Log(L">> START");
    DebugSection(DisassemblySectionType::SYSTEM_EQUATES);

    // Fill referenced flag in lines for SYSTEM labels without offset (sta LABEL)
    SetSystemEquateLinesReferencedBySystemAddress();
    Debug::Log(L">> AFTER CheckSystemLabelReferences");
    DebugSection(DisassemblySectionType::SYSTEM_EQUATES);

    // Fill referenced flag in lines for system equates with address (sta LABEL+n)
    for (auto& equate : workspace->GetSystemEquateList()->GetEquates()) {
        if (equate->HasReferences()) {
            SetNearestSystemEquateLineReferencedByAddress(equate->GetLabelValue());
        }
    }
    Debug::Log(L">> AFTER SetSystemEquateReferenceFlag");
    DebugSection(DisassemblySectionType::SYSTEM_EQUATES);

    AdjustSegmentFirstLineNumber();

    // segmentList->SegmentFreeAllSymbols();     // Keep symbols for code trace
    // segmentList->FreeAllFixups();             // Keep for error analysis
    // segmentList->FreeAllFixupAddressLabels(); // Keep for error analysis
    // segmentList->FreeAllAddresses();          // Keep for error analysis

   // Enable the line below to get debug content for the disassembly control.
   // DisassemblyResultTest::GenerateDisassemblyResult(*workspace->GetDisassemblyResult(), 200);

}

/*
** Generate lines for system or user equates.
*/
void Disassembly::GenerateEquates(DisassemblySectionType disassemblySectionType, const EquateList& equateList) {

    for (const auto& equate : equateList.GetEquates()) {
        switch (equate->GetType()) {
        case EquateType::UNKNOWN:
            throw std::runtime_error("Invalid access");

        case  EquateType::EMPTY:
            AddLine(L"", disassemblySectionType);
            break;

        case  EquateType::COMMENT:
            AddComment(equate->GetComment(), disassemblySectionType);
            break;

        case EquateType::LABEL: {
            // Ignoring those relative to a base label.
            if (!equate->IsRange()) {
                AddLabel(equate->GetLabel(), equate->GetLabelValue(), disassemblySectionType, equate->GetComment());
            }
            break;
        }
        }
    }
}

/*
** Generate lines for all code equates.
*/
void Disassembly::GenerateCodeEquates() {

    const auto& segmentList = workspace->GetSegmentList();
    const auto& segment = segmentList->GetGlobalSegment();
    AddressLabelList::AddressLabelVector addressLabelsVector;
    segment->GetAddressLabels()->Enumerate(addressLabelsVector);
    for (const auto& addressLabel : addressLabelsVector) {
        wsprintf(String::szBuffer, L"L%04hX", addressLabel->GetAddress());
        AddLabel(String::Format(), addressLabel->GetAddress(), DisassemblySectionType::CODE_EQUATES, String::Empty());
    }
}

