#pragma once

#include "Syntax.h"

#include "Equate.h"
#include "MemoryType.h"
#include "SegmentTypes.h"
#include "Memory.h"
#include "DisassemblySectionType.h"
#include "DisassemblyLineWriter.h"
#include "DisassemblyResult.h"
#include "DisassemblySection.h"
#include "DisassemblyOpcodeBuffer.h"

class EquateList;
class Instruction;
class MemoryInspector;
class Fixup;
class Profile;
class Workspace;
class DIS_LINE;
class DisassemblyProgressMonitor;
class DisassemblyWriter;
class MemoryBlockIterator;

class Disassembly {
    friend class DisassemblyWriter;

public:
    Disassembly();

    void SetWorkspace(Workspace& workspace);
    void SetProgressMonitor(DisassemblyProgressMonitor& progressMonitor);

    void StartDisassembly();
    void DisassembleInternal(); // Called from DisassemblyProgressMonitor

    // TODO: Move to MainDisassembly/Segment?
    static bool IsInstructionWithImmediate(const Workspace& workspace, SEGMENT_INDEX segmentIndex, Memory::offset offset, byte& immediateValue, MemoryType& immediateMemoryType);

private:

    Workspace* workspace;
    const Profile* profile;
    DisassemblyResult* result;

    DisassemblyProgressMonitor* disassemblyProgressMonitor;
    int pass;

    byte returnCharacter;

    MemoryBlockIterator* memoryBlockIterator;
    bool bDisNewSegment;

    DisassemblyOpcodeBuffer opcodeBuffer;

    SEGMENT_INDEX markNextSegmentIndex;
    SEGMENT_INDEX markSegmentIndex;
    Memory::offset markOffset;
    Memory::size   markSize;

    DisassemblyLineWriter lineWriter;

    // The next two fields are initilized in DisInit() and used in AddLine().
    Memory::address absoluteAddress = 0;
    Memory::address systemAddress = 0x1234;

    // Higher level operations
    void AddLabelWithAddress(wstring_view label, Memory::address wAddr, DisassemblySectionType bufferype, Memory::address wSystemAddr, wstring_view comment);
    void AddLabel(wstring_view label, Memory::address wAddr, DisassemblySectionType disassemblySectionType, wstring_view comment);
    void AddLabelValue(wstring_view label, Memory::address wAddr, DisassemblySectionType disassemblySectionType, wstring_view comment);
    void AddComment(wstring_view comment, DisassemblySectionType disassemblySectionType);
    void GenerateUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size);
    void AddUserComment(wstring_view comment, DisassemblySectionType disassemblySectionType);

    void SetSystemEquateLinesReferencedBySystemAddress();
    void SetNearestSystemEquateLineReferencedByAddress(Memory::address wAddr);

    // Line operations
    void AddLineInBuffer(const DIS_LINE& disLine, const wchar_t* szLine, DIS_BUFFER* pBuf); // Use wstring_view?
    void AddEmptyCommentLine(DisassemblySectionType disassemblySectionType = DisassemblySectionType::CODE_LINES);
    void AddLine(const wchar_t* szLine, DisassemblySectionType disassemblySectionType = DisassemblySectionType::CODE_LINES);
    void AddLineWriter(DisassemblySectionType disassemblySectionType = DisassemblySectionType::CODE_LINES);


    void SetSegmentFirstLineNumber(SEGMENT_INDEX segmentIndex);
    void AdjustSegmentFirstLineNumber();

    void GenerateSDXSymbolDefinitions();
    bool IsCancelled();

    void AddOrgOrBlock(SEGMENT_INDEX segmentIndex, Memory::address wPC);

    void SetPass(int pass, wstring_view text);
    void Pass2();
    void Pass3();
    void Pass4();
    void Pass5();

    void CreateMemoryBlockIterator(const Segment& segment);
    void ClearMemoryBlockIterator();

    bool DisInit(SEGMENT_INDEX& segmentIndex, Memory::address& pc);
    void DisGetNextByte(byte& bByte, MemoryType& memoryType, SEGMENT_INDEX& segmentIndex, Memory::address& pc);

    void GenerateEquates(DisassemblySectionType disassemblySectionType, const EquateList& equateList);
    void GenerateCodeEquates();

    void DebugSection(DisassemblySectionType disassemblySectionType);

};
