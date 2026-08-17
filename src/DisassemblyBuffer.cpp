#include "Debug.h"
#include "DisassemblyBuffer.h"
#include "DisassemblyLine.h"
#include "Strings.h"

static constexpr size_t DIS_LINE_SIZE = sizeof(DIS_LINE);

DIS_BUFFER::DIS_BUFFER() :
    lineCount(0),
    index(0)
{
    disLines = (DIS_LINE*)(buffer.get());
}

DIS_BUFFER::LineCount DIS_BUFFER::GetLineCount() const {
    return lineCount;
}

DIS_LINE* DIS_BUFFER::GetDisLine() {
    return disLines;
}

const DIS_LINE* DIS_BUFFER::GetConstDisLine() const {
    return disLines;
}

bool DIS_BUFFER::CanAddLine(const wchar_t* line) const {
    const auto newIndex = index + DIS_LINE_SIZE + (wcslen(line) + 1) * sizeof(wchar_t);
    return newIndex <= MAX_BUFFER_SIZE;
}

void DIS_BUFFER::AddLine(const DIS_LINE& disLine, const wstring& line) {
    AddLine(disLine, line.c_str());
}


void DIS_BUFFER::AddLine(const DIS_LINE& disLine, const wchar_t* line) {

    const auto lineSize = wcslen(line);
    if (lineSize > 1024 - 6) { // 4 digit line number plus space plus zero-termination, TODO: have constant
        throw std::runtime_error("Line too long");
    }
    // Debug::Log(line);
    // Debug::Log(String::Format("buffer={0}, lineCount={1}, index={2}", std::to_hex_string((uintmax_t)disLines), std::to_string(lineCount), std::to_string(index)));

    auto destDisLine = (DIS_LINE*)(((byte*)disLines) + index);
    memcpy(destDisLine, &disLine, DIS_LINE_SIZE);
    auto destLine = (wchar_t*)(((byte*)destDisLine) + DIS_LINE_SIZE);
    wcscpy(destLine, line);
    // TODO: Remove when stable String::AssertASCII(line);
    index += DIS_LINE_SIZE + (lineSize + 1) * sizeof(wchar_t);
    lineCount++;
}
