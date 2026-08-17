#pragma once

#include "Syntax.h"
#include "DisassemblyResultTypes.h"
#include "DisassemblyBuffer.h"
#include <gsl/gsl>

enum class DisassemblySectionType;
class DisassemblyResult;
class DisassemblySection;
class DisassemblyLine;

class DisassemblyResultAbstractLineIterator {

public:
    bool HasNext() const;

protected:
    DisassemblyResultAbstractLineIterator(size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount);

    size_t startIndex;
    size_t endIndex;

    DisassemblyResultLineCount lineCount; // Cached for performance
    DisassemblyResultLineNumber lineNumber;

    size_t sectionIndex;
    size_t bufferIndex;

    DIS_BUFFER::LineNumber bufferLineNumber;
};

class DisassemblyResultLineIterator : public DisassemblyResultAbstractLineIterator {

public:
    DisassemblyResultLineIterator(DisassemblyResult& result, size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount); // Public because of the unique_ptr

    gsl::not_null<DIS_LINE*> Next();

    DIS_BUFFER* GetDisBuffer() const;
    gsl::not_null<DIS_LINE*> GetDisLine() const;

private:

    DisassemblyResult* result;
    DisassemblySection* section;
    DIS_BUFFER* buffer;
    DIS_LINE* disLine;

    void InitSection(size_t newSectionIndex);
    void InitBuffer(size_t newBufferIndex);
};

class DisassemblyResultConstLineIterator : public DisassemblyResultAbstractLineIterator {

public:
    DisassemblyResultConstLineIterator(const DisassemblyResult& result, size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount); // Public because of the unique_ptr

    gsl::not_null<const DIS_LINE*> Next();

    const DIS_BUFFER* GetDisBuffer() const;
    gsl::not_null<const DIS_LINE*> GetDisLine() const;

private:

    const DisassemblyResult* result;
    const DisassemblySection* section;
    const DIS_BUFFER* buffer;
    const DIS_LINE* disLine;

    void InitSection(size_t newSectionIndex);
    void InitBuffer(size_t newBufferIndex);
};