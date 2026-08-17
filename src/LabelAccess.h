#pragma once

#include "Syntax.h"
#include <vector>

enum class LabelAccess {
    UNKNOWN =0,
    READ = 1,
    WRITE = 2,
    READ_WRITE = 3,
    IMMEDIATE = 4,

};


struct LabelAccessInfo {
    const LabelAccess labelAccess;
    const wstring key;
    const wstring text;

    LabelAccessInfo(const LabelAccess labelAccess, const wstring_view key, const wstring_view text) :
        labelAccess{ labelAccess }, key{ key }, text{ text }
    {
    }
};

class LabelAccessFactory {
public:
    static const LabelAccessInfo& GetInfo(const wstring_view key);
    static const LabelAccessInfo& GetInfo(const LabelAccess labelAccess);
    static int GetIndex(const LabelAccess labelAccess, const std::vector<LabelAccess> labelAccesss, int defaultIndex);

    static const wchar_t* GetQualifier(LabelAccess labelAccess);
};
