#pragma once

#include "Syntax.h"
#include <vector>

enum class EquateType {
    UNKNOWN,
    EMPTY,
    COMMENT,
    LABEL
};


struct EquateTypeInfo {
    const EquateType equateType;
    const wstring key;
    const wstring text;

    EquateTypeInfo(const EquateType equateType, const wstring_view key, const wstring_view text) :
        equateType{ equateType }, key{ key }, text{ text }
    {
    }
};

class EquateTypeFactory {
public:
    static const EquateTypeInfo& GetInfo(const wstring_view key);
    static const EquateTypeInfo& GetInfo(const EquateType equateType);
    static int GetIndex(const EquateType equateType, const std::vector<EquateType> equateTypes, int defaultIndex);
};
