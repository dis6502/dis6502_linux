#include "LabelAccess.h"
#include <algorithm>
#include <map>


const LabelAccessInfo& LabelAccessFactory::GetInfo(wstring_view key) {
    static std::map<wstring_view, LabelAccess> lookup = {
        { L"READ", LabelAccess::READ },
        { L"WRITE", LabelAccess::WRITE },
        { L"READ_WRITE", LabelAccess::READ_WRITE },
        { L"IMMEDIATE", LabelAccess::IMMEDIATE },

    };

    LabelAccess LabelAccess = LabelAccess::UNKNOWN;
    try {
        LabelAccess = lookup.at(key);
    }
    catch (const std::out_of_range&) {}

    return GetInfo(LabelAccess);
}

const LabelAccessInfo& LabelAccessFactory::GetInfo(const LabelAccess labelAccess) {
    static auto UNKOWN = LabelAccessInfo(LabelAccess::UNKNOWN, L"UNKNOWN", L"Unknown");
    static auto READ = LabelAccessInfo(LabelAccess::READ, L"READ", L"Read");
    static auto WRITE = LabelAccessInfo(LabelAccess::WRITE, L"WRITE", L"Write");
    static auto READ_WRITE = LabelAccessInfo(LabelAccess::READ_WRITE, L"READ_WRITE", L"Read-Write");
    if (labelAccess == LabelAccess::READ)
        return READ;
    else if (labelAccess == LabelAccess::WRITE)
        return WRITE;
    else if (labelAccess == LabelAccess::READ_WRITE)
        return READ_WRITE;

    return UNKOWN;
}

int LabelAccessFactory::GetIndex(const LabelAccess labelAccess, const std::vector<LabelAccess> labelAccesss, int defaultIndex) {
    auto it = std::find(labelAccesss.begin(), labelAccesss.end(), labelAccess);
    if (it == labelAccesss.end()) {
        return 0;
    }
    return std::distance(labelAccesss.begin(), it);
}

const wchar_t* LabelAccessFactory::GetQualifier(LabelAccess labelAccess) {
    const wchar_t* szResult;
    switch (labelAccess) {

    case LabelAccess::READ:
        szResult = L"<";
        break;

    case LabelAccess::WRITE:
        szResult = L">";
        break;

    case LabelAccess::READ_WRITE:
        szResult = L"=";
        break;

    case LabelAccess::IMMEDIATE:
        szResult = L"#";
        break;

    default:
        szResult = L"?";
        break;
    }
    return szResult;
}
