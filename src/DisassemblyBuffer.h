#pragma once

#include "Syntax.h"
#include "Byte.h"

class DIS_LINE;

/*
** Structure of one buffer. This is a linked list with only a pointer on the next buffer.
*/
class DIS_BUFFER {
public:

    typedef unsigned int LineCount;
    typedef unsigned int LineNumber;

    DIS_BUFFER();

    bool CanAddLine(const wchar_t* line) const;
    void AddLine(const DIS_LINE& disLine, const wstring& line);
    void AddLine(const DIS_LINE& disLine, const wchar_t* line);
    LineCount GetLineCount() const;

    DIS_LINE* GetDisLine(); // First line
    const DIS_LINE* GetConstDisLine() const; // First line

private:
    /*
     ** maximum size of a buffer in bytes
    */
    static constexpr size_t MAX_BUFFER_SIZE = 32500;
    std::unique_ptr<byte[]> buffer = std::make_unique<byte[]>(MAX_BUFFER_SIZE);
    LineCount lineCount;
    size_t index;
    DIS_LINE* disLines;
};

