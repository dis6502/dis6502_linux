#include "DisassemblyLineWriter.h"
#include "Disassembly.h"
#include "Workspace.h"
#include "Profile.h"
#include "String.h"
#include <gsl/gsl>

DisassemblyLineWriter::DisassemblyLineWriter(size_t lineBufferSize) {
    this->lineBuffer = new wchar_t[lineBufferSize];
    this->maxIndex = lineBufferSize - 1; // Zero termination
    workspace = nullptr;
    profile = nullptr;

    index = maxIndex;
    Clear();
}

DisassemblyLineWriter::~DisassemblyLineWriter() {
    delete[] lineBuffer;

}
void DisassemblyLineWriter::Init(const Workspace& workspace) {
    this->workspace = &workspace;
    profile = workspace.GetConstProfile();

    // Cache formatter profile infos.
    directiveForceAbsolute = profile->directiveForceAbsolute;
    if (profile->showLowerCaseInstructions) {
        directiveForceAbsolute = String::ToLower(directiveForceAbsolute);
    }
}

gsl::not_null<const wchar_t*> DisassemblyLineWriter::GetLineBuffer() const {
    return lineBuffer;
}

DisassemblyLineWriter& DisassemblyLineWriter::Clear() {
    wmemset(lineBuffer, 0, index + 1);
    index = 0;
    return *this;

}

DisassemblyLineWriter& DisassemblyLineWriter::Char(const wchar_t value) {
    if (index >= maxIndex) {
        throw std::runtime_error("Line size exceeds line buffer");
    }
    if (value < 32 || value >127) {
        throw std::runtime_error("Invalid character");
    }
    lineBuffer[index++] = value;
    return *this;
}

DisassemblyLineWriter& DisassemblyLineWriter::CString(const wchar_t* value) {
    while (*value) {
        Char(*(value++));
    }
    return *this;
}

DisassemblyLineWriter& DisassemblyLineWriter::String(wstring_view value) {
    auto const size = value.size();
    for (size_t i = 0; i < size; i++) {
        Char(value.at(i));
    }
    return *this;
}
DisassemblyLineWriter& DisassemblyLineWriter::String(const wstring& value) {
    auto const size = value.size();
    for (size_t i = 0; i < size; i++) {
        Char(value.at(i));
    }
    return *this;
}
DisassemblyLineWriter& DisassemblyLineWriter::Comment(wstring_view comment) {
    String(profile->commentPrefix);
    if (!comment.empty()) {
        Space();
        String(comment);
    }
    return *this;
}

DisassemblyLineWriter& DisassemblyLineWriter::Space() {
    return Char(L' ');
}

DisassemblyLineWriter& DisassemblyLineWriter::SpaceUntil34() {

    while (index < 34) {
        Space();
    }
    return *this;
}

DisassemblyLineWriter& DisassemblyLineWriter::Decimal(const unsigned int value) {
    wchar_t buffer[11]; // 4294967295
    wsprintf(buffer, L"%u", value);
    return CString(buffer);
}

DisassemblyLineWriter& DisassemblyLineWriter::Number(const unsigned int value) {
    if (profile->useHexNotation) {
        wchar_t buffer[11];
        wsprintf(buffer, L"%s%04X", profile->hexNotationPrefix.c_str(), value);
        return CString(buffer);
    }
    else {
        return Decimal(value);
    }
}

DisassemblyLineWriter& DisassemblyLineWriter::ByteNumber(const Memory::byte value) {
    if (profile->useHexNotation) {
        wchar_t buffer[11];
        wsprintf(buffer, L"%s%02X", profile->hexNotationPrefix.c_str(), value);
        return CString(buffer);
    }
    else {
        return Decimal(value);
    }
}

DisassemblyLineWriter& DisassemblyLineWriter::Byte(const Memory::byte value) {
    if (profile->useHexNotation) {
        wchar_t buffer[11];
        wsprintf(buffer, L"%s%02X", profile->hexNotationPrefix.c_str(), value);
        return CString(buffer);
    }
    else {
        return Decimal(value);
    }
}

DisassemblyLineWriter& DisassemblyLineWriter::Address(const Memory::address address) {
    return Number(address);
}

DisassemblyLineWriter& DisassemblyLineWriter::Label(const wchar_t* szLabel) {
    return CString(szLabel);
}


DisassemblyLineWriter& DisassemblyLineWriter::Label(const wstring& label) {
    return Label(label.c_str());
}

DisassemblyLineWriter& DisassemblyLineWriter::LabelOrZeroPageAddress(SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode) {
    if (wAddr >= 0x0100) {
        throw std::runtime_error("Address is not on zero page");
    }
    auto label = segmentList.GetLabelAtAddress(segmentIndex, wPC, wAddr, cType, opcode);
    if (!label.empty()) {
        return Label(label);
    }
    return Byte((Memory::byte)(wAddr & 0xff));
}
DisassemblyLineWriter& DisassemblyLineWriter::LabelOrAddress(SegmentList& segmentList, SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode) {
    auto label = segmentList.GetLabelAtAddress(segmentIndex, wPC, wAddr, cType, opcode);
    if (!label.empty()) {
        return Label(label.c_str());
    }
    return Address(wAddr);
}

DisassemblyLineWriter& DisassemblyLineWriter::AlignInstructions() {
    if (profile->alignInstructions) {
        auto const labelLength = wcslen(lineBuffer);
        auto const maxLength = std::max<size_t>(labelLength + 1, 12);
        for (auto len = labelLength; len < maxLength; len++) {
            Space();
        }
    }
    return *this;
}

DisassemblyLineWriter& DisassemblyLineWriter::Instruction(const gsl::not_null<const Segment*> segment, byte opcode, Memory::address wAddr) {
    const auto& instructionSet = workspace->GetInstructionSet(segment->processorType);
    const bool isDualMode = (wAddr < 0x100) && (instructionSet->IsDualAddressingMode(opcode)) && (!profile->showZPAbsoluteAsByte) && (!directiveForceAbsolute.empty());
    const auto opcodeName = instructionSet->GetInstruction(opcode).GetName();

    if (profile->showLowerCaseInstructions) {
        const auto length = opcodeName.length();
        for (size_t i = 0; i < length; i++) {
            Char(tolower(opcodeName.at(i)));
        }
    }
    else {
        String(opcodeName);
    }

    if (isDualMode) {
        String(directiveForceAbsolute);
    }
    return *this;
}