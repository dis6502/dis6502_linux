#pragma once

#include "Syntax.h"
#include "DisassemblySectionType.h"
#include "DisassemblySection.h"
#include "SegmentTypes.h"
#include "Memory.h"
#include <vector>
#include <gsl/gsl>

#include "DisassemblyResultTypes.h"
#include "DisassemblyResultIterators.h"

class DisassemblyResult {

public:
    typedef DisassemblyResultLineCount LineCount;  // Starting with 0
    typedef DisassemblyResultLineNumber LineNumber; // Starting with 1

    DisassemblyResult() noexcept;

    const std::vector<DisassemblySectionType>& GetSectionTypes() const;

    std::unique_ptr<DisassemblyResultLineIterator> CreateLineIterator(); // All buffers
    std::unique_ptr<DisassemblyResultLineIterator> CreateLineIterator(DisassemblySectionType disassemblySectionType); // Single buffer

    std::unique_ptr<DisassemblyResultConstLineIterator> CreateConstLineIterator() const; // All buffers
    std::unique_ptr<DisassemblyResultConstLineIterator> CreateConstLineIterator(DisassemblySectionType disassemblySectionType) const; // Single buffer

    void Clear();
    size_t GetSectionCount() const;
    DisassemblySection* GetSection(size_t index);
    const DisassemblySection* GetConstSection(size_t index) const;
    DisassemblySection* GetSection(DisassemblySectionType disassemblySectionType);

    void ClearSection(DisassemblySectionType disassemblySectionType); 
    // Clear system and user equate definitions.
    void ClearEquateSections();


    gsl::not_null<DisassemblySection*> AllocSection(DisassemblySectionType disassemblySectionType); 

    LineCount GetLineCount() const;

    DIS_LINE* SelectLine(LineNumber lineNumber);
    LineNumber SelectLine(SEGMENT_INDEX segmentIndex, Memory::offset offset); // Returns 0 if not found
    bool ExtendSelectionTo(SEGMENT_INDEX segmentIndex, Memory::offset offset);

    bool FindAndSelectLines(bool first, DisassemblyResult::LineNumber& findFirstLineNumber, wstring_view findString);

    void FindOffsetAtStartOfInstruction(SEGMENT_INDEX segmentIndex, Memory::offset& offset, Memory::size& size) const;

private:

    static std::vector<DisassemblySectionType> sectionTypes;

    std::vector<std::unique_ptr<DisassemblySection>> sections;
};