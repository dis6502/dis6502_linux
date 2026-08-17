#pragma once

#include "Memory.h"
#include "Segment.h"
#include "XRef.h"

/*
** Structure of each line in the disassembly
** The line of text begins with this structure to store the offset in a segment to be
** able to link memory inspector, the disassembly window and optionally the xref window
*/
class DisassemblySection;
class DisassemblyResult;
class DisassemblyResultLineIterator;
class DisassemblyResultConstLineIterator;

class DIS_LINE {
private:
    const DisassemblySection* section;

public:

    DIS_LINE(DisassemblySection& section);

    SEGMENT_INDEX            segmentIndex;   // Segment index where the instruction starts
    Memory::offset           offset;         // Offset of the instruction in the segment
    Memory::size             size;           // Size of the instruction
    XRef::LineNumber         xrefLineNumber; // Line number of this line in the XRef search list window
    bool                     selected;       // Displayed in yellow background
    bool                     referenced;     // Displayed in grey if not referenced
    Memory::address          address;        // Absolute address to display as comment (in addition to the label)
    Memory::address          systemAddress;  // Address is filled only for system equates

    gsl::not_null<const DisassemblySection*> GetSection() const;
    DisassemblyResultLineNumber GetLineNumber() const;

    const wchar_t* GetLine() const;

    DIS_LINE* GetNext();
    const DIS_LINE* GetConstNext() const;

    wstring ToString() const;

private:
    friend class DisassemblyResultLineIterator;
    friend class DisassemblyResultConstLineIterator;

    DisassemblyResultLineNumber lineNumber;

    void SetLineNumber(DisassemblyResultLineNumber lineNumber);

};