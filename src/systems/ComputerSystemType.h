#pragma once

#include "Syntax.h"

enum class ComputerSystemType {
    ATARI5200,
    ATARI800,
    C64,
    ORIC,
    UNKNOWN
};

class ComputerSystemTypeInfo {
public:
    const ComputerSystemType type;
    const wstring id;
    const wstring text;
    const wstring fileName;

    ComputerSystemTypeInfo(ComputerSystemType type, wstring_view key, wstring_view text, wstring_view fileName);
};
