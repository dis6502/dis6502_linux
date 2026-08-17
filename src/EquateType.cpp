#include "EquateType.h"
#include <algorithm>
#include <map>


const EquateTypeInfo& EquateTypeFactory::GetInfo(wstring_view key) {
    static std::map<wstring_view, EquateType> lookup = {
        { L"EMPTY", EquateType::EMPTY },
        { L"COMMENT", EquateType::COMMENT },
        { L"LABEL", EquateType::LABEL },
    };

    EquateType EquateType = EquateType::UNKNOWN;
    try {
        EquateType = lookup.at(key);
    }
    catch (const std::out_of_range&) {}

    return GetInfo(EquateType);
}

const EquateTypeInfo& EquateTypeFactory::GetInfo(const EquateType equateType) {
    static auto UNKNOWN = EquateTypeInfo(EquateType::UNKNOWN, L"UNKNOWN", L"Unknown");
    static auto EMPTY= EquateTypeInfo(EquateType::EMPTY, L"EMPTY", L"Empty Line");
    static auto COMMENT = EquateTypeInfo(EquateType::COMMENT, L"COMMENT", L"Comment Line");
    static auto LABEL = EquateTypeInfo(EquateType::COMMENT, L"LABEL", L"Label Line");

    if (equateType == EquateType::EMPTY)
        return EMPTY;
    else if (equateType == EquateType::COMMENT)
        return COMMENT;
    else if (equateType == EquateType::LABEL)
        return LABEL;

    return UNKNOWN;
}

int EquateTypeFactory::GetIndex(const EquateType equateType, const std::vector<EquateType> equateTypes, int defaultIndex) {
    auto it = std::find(equateTypes.begin(), equateTypes.end(), equateType);
    if (it == equateTypes.end()) {
        return 0;
    }
    return std::distance(equateTypes.begin(), it);
}