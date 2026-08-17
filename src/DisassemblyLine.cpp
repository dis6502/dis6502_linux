#include "DisassemblyLine.h"

DIS_LINE::DIS_LINE(DisassemblySection& section) :
    section(&section),
    segmentIndex(0),
    offset(0),
    size(0),
    xrefLineNumber(0),
    selected(false),
    referenced(false),
    address(0),
    systemAddress(0),
    lineNumber(0)
{
}

gsl::not_null<const DisassemblySection*> DIS_LINE::GetSection() const {
    return section;
}

DisassemblyResultLineNumber DIS_LINE::GetLineNumber() const {
    if (lineNumber == 0) {
        throw std::runtime_error("Line number not yet set");
    }
    return lineNumber;
}

const wchar_t* DIS_LINE::GetLine() const {
    byte* disLine = (byte*)this;
    disLine += sizeof(DIS_LINE);
    return (const wchar_t*)disLine;
}

DIS_LINE* DIS_LINE::GetNext() {
    auto disLine = GetLine();
    disLine += (wcslen(disLine) + 1);
    return (DIS_LINE*)disLine;
}

const DIS_LINE* DIS_LINE::GetConstNext() const {
    auto disLine = GetLine();
    disLine += (wcslen(disLine) + 1);
    return (DIS_LINE*)disLine;
}

void DIS_LINE::SetLineNumber(DisassemblyResultLineNumber lineNumber) {
    this->lineNumber = lineNumber;
}

wstring DIS_LINE::ToString() const {
    wstringstream stream;
    stream << L"segmentIndex=" << segmentIndex << ", offset=" << Memory::offset_to_hex_string(offset) << L", size=" << Memory::size_to_hex_string(size) + L", xrefLineNumber=" << xrefLineNumber << L", selected=" << std::to_wstring(selected) << L", referenced=" << std::to_wstring(referenced) << L", address=" << Memory::address_to_hex_string(address) << L", systemAddress=" << Memory::address_to_hex_string(systemAddress) << L", lineNumber=" << lineNumber << L", line=" << GetLine();
    return stream.str();
}