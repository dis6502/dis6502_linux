#include "DisassemblySection.h"


size_t DisassemblySection::GetIndex(DisassemblySectionType disassemblySectionType) {
    switch (disassemblySectionType) {
    case DisassemblySectionType::SYSTEM_EQUATES:
        return 0;
    case DisassemblySectionType::USER_EQUATES:
        return 1;
    case DisassemblySectionType::CODE_EQUATES:
        return 2;
    case DisassemblySectionType::CODE_LINES:
        return 3;
    default:
        throw std::runtime_error("Invaid disassemblySectionType");
    }
}

DisassemblySectionType DisassemblySection::GetType(size_t index) {
    switch (index) {
    case 0:
        return DisassemblySectionType::SYSTEM_EQUATES;
    case 1:
        return DisassemblySectionType::USER_EQUATES;
    case 2:
        return  DisassemblySectionType::CODE_EQUATES;
    case 3:
        return DisassemblySectionType::CODE_LINES;
    default:
        throw std::runtime_error("Invaid index");
    }
}

wstring DisassemblySection::GetText(DisassemblySectionType disassemblySectionType) {
    switch (disassemblySectionType) {
    case DisassemblySectionType::SYSTEM_EQUATES:
        return L"System equates";

    case DisassemblySectionType::USER_EQUATES:
        return L"User equates";

    case DisassemblySectionType::CODE_EQUATES:
        return L"Code equates";

    case DisassemblySectionType::CODE_LINES:
        return L"Start of code";

    default:
        throw std::runtime_error("Undefined disassemblySectionType");
    }

}

DisassemblySection::DisassemblySection(const DisassemblyResult& disassemblyResult, DisassemblySectionType disassemblySectionType) :
    disassemblyResult(&disassemblyResult), disassemblySectionType(disassemblySectionType) {
}

const DisassemblyResult* DisassemblySection::GetDisassemblyResult() const {
    return disassemblyResult;
}

DisassemblySectionType DisassemblySection::GetType() const {
    return disassemblySectionType;
}

void DisassemblySection::ClearBuffers() {
    buffers.clear();
}

size_t DisassemblySection::GetBufferCount() const {
    return buffers.size();
}

DIS_BUFFER* DisassemblySection::GetBuffer(size_t bufferIndex) {
    if (buffers.size() < bufferIndex + 1) {
        return nullptr;
    }
    return buffers.at(bufferIndex).get();
}

const DIS_BUFFER* DisassemblySection::GetConstBuffer(size_t bufferIndex) const {
    if (buffers.size() < bufferIndex + 1) {
        return nullptr;
    }
    return buffers.at(bufferIndex).get();
}

/*
** find the last buffer of a list.
*/
DIS_BUFFER* DisassemblySection::FindLastBuffer() {

    // add this buffer to the end of the list.
    if (buffers.empty()) {
        return nullptr;
    }
    return GetBuffer(buffers.size() - 1);
}

gsl::not_null<DIS_BUFFER*> DisassemblySection::AllocBuffer() {

    // add this buffer to the end of the list.
    buffers.push_back(std::make_unique<DIS_BUFFER>());

    return GetBuffer(buffers.size() - 1);
}

DisassemblySection::LineCount DisassemblySection::GetLineCount() const {
    LineCount lineCount = 0;
    for (const auto& buffer : buffers) {
        lineCount += buffer->GetLineCount();
    }
    return lineCount;
}
