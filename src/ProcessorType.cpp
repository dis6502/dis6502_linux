#include "ProcessorType.h"
#include <algorithm>

#include <map>


const ProcessorTypeInfo& ProcessorTypeFactory::GetInfo(wstring_view key) {
    static std::map<wstring_view, ProcessorType> lookup = {
        { L"MOS6502", ProcessorType::MOS6502 },
        { L"MOS65C02", ProcessorType::MOS65C02 },
    };

    ProcessorType processorType = ProcessorType::UNKNOWN;
    try {
        processorType = lookup.at(key);
    }
    catch (const std::out_of_range&) {}

    return GetInfo(processorType);
}

const ProcessorTypeInfo& ProcessorTypeFactory::GetInfo(const ProcessorType processorType) {
    static auto UNKNOWN = ProcessorTypeInfo(ProcessorType::UNKNOWN, L"UNKNOWN", L"Unknown");
    static auto MOS6502 = ProcessorTypeInfo(ProcessorType::MOS6502, L"MOS6502", L"MOS 6502");
    static auto MOS65C02 = ProcessorTypeInfo(ProcessorType::MOS65C02, L"MOS65C02", L"MOS 65C02");
    if (processorType == ProcessorType::MOS6502)
        return MOS6502;
    else if (processorType == ProcessorType::MOS65C02)
        return MOS65C02;

    return UNKNOWN;
}

int ProcessorTypeFactory::GetIndex(const ProcessorType processorType, const std::vector<ProcessorType> processorTypes, int defaultIndex) {
    auto it = std::find(processorTypes.begin(), processorTypes.end(), processorType);
    if (it == processorTypes.end()) {
        return 0;
    }
    return std::distance(processorTypes.begin(), it);
}