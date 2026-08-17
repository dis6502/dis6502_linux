#pragma once

#include "Syntax.h"
#include "DisassemblySectionType.h"
#include <vector>
#include "DisassemblyBuffer.h"
#include <gsl/gsl>

class DisassemblyResult;

class DisassemblySection {
public:

    typedef unsigned int LineCount;

    static size_t GetIndex(DisassemblySectionType disassemblySectionType);
    static DisassemblySectionType GetType(size_t index);
    static wstring GetText(DisassemblySectionType disassemblySectionType);

    DisassemblySection(const DisassemblyResult& disassemblyResult, DisassemblySectionType disassemblySectionType);
    const DisassemblyResult* GetDisassemblyResult() const;
    DisassemblySectionType GetType() const;

    void ClearBuffers();
    size_t GetBufferCount() const;
    DIS_BUFFER* GetBuffer(size_t bufferIndex);
    const DIS_BUFFER* GetConstBuffer(size_t bufferIndex) const;

    DIS_BUFFER* FindLastBuffer(); 
    gsl::not_null<DIS_BUFFER*> AllocBuffer();

    LineCount GetLineCount() const;
    std::vector<std::unique_ptr<DIS_BUFFER>> buffers;

private:
    const DisassemblyResult* disassemblyResult;
    const DisassemblySectionType disassemblySectionType;
};