#include "DisassemblyResultIterators.h"

#include "DisassemblyLine.h"
#include "DisassemblySectionType.h"
#include "DisassemblySection.h"
#include "DisassemblyResult.h"
#include "DisassemblyBuffer.h"


DisassemblyResultAbstractLineIterator::DisassemblyResultAbstractLineIterator(size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount) :
    startIndex(startIndex),
    endIndex(endIndex),
    lineCount(lineCount),
    lineNumber(0),
    sectionIndex(0),
    bufferIndex(0),
    bufferLineNumber(0)
{
}


bool DisassemblyResultAbstractLineIterator::HasNext() const {
    return lineNumber < lineCount;
}




DisassemblyResultLineIterator::DisassemblyResultLineIterator(DisassemblyResult& result, size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount) :
    DisassemblyResultAbstractLineIterator(startIndex, endIndex, lineCount),
    result(&result),
    section(nullptr),
    buffer(nullptr),
    disLine(0)
{
}

gsl::not_null<DIS_LINE*> DisassemblyResultLineIterator::Next() {
    if (!HasNext()) {
        throw std::runtime_error("No next line");
    }
    if (lineNumber == 0) {

        InitSection(startIndex);

    }
    else {
        disLine = disLine->GetNext();
        bufferLineNumber++;
        if (bufferLineNumber > buffer->GetLineCount()) {
            bufferIndex++;
            if (bufferIndex < section->GetBufferCount()) {
                InitBuffer(bufferIndex);
            }
            else {
                InitSection(sectionIndex+1);
            }


        }
    }
    lineNumber++;
    disLine->SetLineNumber(lineNumber);
    return disLine;
}

    void DisassemblyResultLineIterator::InitSection(size_t newSectionIndex) {
        sectionIndex = newSectionIndex;
        section = result->GetSection(sectionIndex);
        while (section == nullptr || section->GetBufferCount() == 0) {
            sectionIndex++;
            if (sectionIndex > endIndex) {
                throw std::runtime_error("No next section");
            }
            section = result->GetSection(sectionIndex);
        }
        InitBuffer(0);
    }

    void DisassemblyResultLineIterator::InitBuffer(size_t newBufferIndex) {
        bufferIndex = newBufferIndex;
        buffer = section->GetBuffer(bufferIndex);

        disLine = buffer->GetDisLine();
        bufferLineNumber = 1;
    }

    DIS_BUFFER* DisassemblyResultLineIterator::GetDisBuffer() const {
        if (lineNumber == 0) {
            throw std::runtime_error("Iterator not ready");
        }
        return buffer;

    }
    gsl::not_null<DIS_LINE*> DisassemblyResultLineIterator::GetDisLine() const {
        if (lineNumber == 0) {
            throw std::runtime_error("Iterator not ready");
        }
        return disLine;

    }

    DisassemblyResultConstLineIterator::DisassemblyResultConstLineIterator(const DisassemblyResult & result, size_t startIndex, size_t endIndex, DisassemblyResultLineCount lineCount) :
        DisassemblyResultAbstractLineIterator(startIndex, endIndex, lineCount),
        result(&result),
        section(nullptr),
        buffer(nullptr),
        disLine(0)
    {
    }

    gsl::not_null<const DIS_LINE*> DisassemblyResultConstLineIterator::Next() {
        if (!HasNext()) {
            throw std::runtime_error("No next line");
        }
        if (lineNumber == 0) {

            InitSection(startIndex);

        }
        else {
            disLine = disLine->GetConstNext();
            bufferLineNumber++;
            if (bufferLineNumber > buffer->GetLineCount()) {
                bufferIndex++;
                if (bufferIndex < section->GetBufferCount()) {
                    InitBuffer(bufferIndex);
                }
                else {
                    InitSection(sectionIndex + 1);
                }


            }
        }
        lineNumber++;
        ((DIS_LINE*)disLine)->SetLineNumber(lineNumber); // Ignore const
        return disLine;
    }

    void DisassemblyResultConstLineIterator::InitSection(size_t newSectionIndex) {
        sectionIndex = newSectionIndex;
        section = result->GetConstSection(sectionIndex);
        while (section == nullptr || section->GetBufferCount() == 0) {
            sectionIndex++;
            if (sectionIndex > endIndex) {
                throw std::runtime_error("No next section");
            }
            section = result->GetConstSection(sectionIndex);
        }
        InitBuffer(0);
    }

    void DisassemblyResultConstLineIterator::InitBuffer(size_t newBufferIndex) {
        bufferIndex = newBufferIndex;
        buffer = section->GetConstBuffer(bufferIndex);

        disLine = buffer->GetConstDisLine();
        bufferLineNumber = 1;
    }

    const DIS_BUFFER* DisassemblyResultConstLineIterator::GetDisBuffer() const {
        if (lineNumber == 0) {
            throw std::runtime_error("Iterator not ready");
        }
        return buffer;

    }
    gsl::not_null<const DIS_LINE*> DisassemblyResultConstLineIterator::GetDisLine() const {
        if (lineNumber == 0) {
            throw std::runtime_error("Iterator not ready");
        }
        return disLine;

    }
