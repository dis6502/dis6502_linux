#pragma once
#include "Syntax.h"
#include <gsl/gsl>
#include "Memory.h"
#include "MemoryType.h"
#include "SegmentList.h"

class Profile;
class Workspace;
class Segment;

class DisassemblyLineWriter final {
public:

    DisassemblyLineWriter(size_t lineBufferSize);
    ~DisassemblyLineWriter();

    // Initialize the state before a disassembly run
    void Init(const Workspace& workspace);

    gsl::not_null<const wchar_t*> GetLineBuffer() const;

    DisassemblyLineWriter& Clear();
    DisassemblyLineWriter& Char(const wchar_t value);
    DisassemblyLineWriter& CString(const wchar_t* value);
    DisassemblyLineWriter& String(wstring_view value);
    DisassemblyLineWriter& String(const wstring& value);
    DisassemblyLineWriter& Comment(wstring_view comment = L"");
    DisassemblyLineWriter& Space();
    DisassemblyLineWriter& SpaceUntil34();

    DisassemblyLineWriter& Decimal(const unsigned int value);
    DisassemblyLineWriter& Number(const unsigned int value);
    DisassemblyLineWriter& ByteNumber(const Memory::byte value);

    DisassemblyLineWriter& Byte(const Memory::byte value);
    DisassemblyLineWriter& Address(const Memory::address address);
    DisassemblyLineWriter& Label(const wchar_t* szLabel);
    DisassemblyLineWriter& Label(const wstring& label);
    DisassemblyLineWriter& LabelOrZeroPageAddress(SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode);
    DisassemblyLineWriter& LabelOrAddress(SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode);

    DisassemblyLineWriter& AlignInstructions();
    DisassemblyLineWriter& Instruction(const gsl::not_null<const Segment*> segment, Memory::byte opcode, Memory::address wAddr = 0xFFFF);

private:
    wchar_t* lineBuffer;
    size_t maxIndex;

    const Workspace* workspace;
    const Profile* profile;
    wstring directiveForceAbsolute;

    size_t index;
};