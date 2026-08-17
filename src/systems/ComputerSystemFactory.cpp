#include <map>

#include "ComputerSystemFactory.h"


ComputerSystemFactory::ComputerSystemFactory() {
    atari5200 = std::make_unique<Atari5200>(*GetComputerSystemTypeInfo(ComputerSystemType::ATARI5200));
    atari5200->Init();
    atari800 = std::make_unique<Atari800>(*GetComputerSystemTypeInfo(ComputerSystemType::ATARI800));
    atari800->Init();
    c64 = std::make_unique<C64>(*GetComputerSystemTypeInfo(ComputerSystemType::C64));
    c64->Init();
    oric = std::make_unique<Oric>(*GetComputerSystemTypeInfo(ComputerSystemType::ORIC));
    oric->Init();
    unknown = std::make_unique<systems::Unknown>(*GetComputerSystemTypeInfo(ComputerSystemType::UNKNOWN));
    unknown->Init();
}

ComputerSystemType ComputerSystemFactory::GetComputerSystemType(wstring_view id) const {
    static std::map<wstring_view, ComputerSystemType> typeLookup = {
        { L"ATARI5200", ComputerSystemType::ATARI5200 },
        { L"ATARI800", ComputerSystemType::ATARI800 },
        { L"C64", ComputerSystemType::C64 },
        { L"ORIC", ComputerSystemType::ORIC }
    };

    ComputerSystemType computerSystemType = ComputerSystemType::UNKNOWN;
    auto i = typeLookup.find(id);
    if (i != typeLookup.end()) {
        computerSystemType = i->second;
    }
    return computerSystemType;
}

gsl::not_null<const ComputerSystemTypeInfo*> ComputerSystemFactory::GetComputerSystemTypeInfo(ComputerSystemType type) const{
    static const auto UNKNOWN = ComputerSystemTypeInfo(ComputerSystemType::UNKNOWN, L"UNKNOWN", L"Unknown", L"Unknown");
    static const auto ATARI800 = ComputerSystemTypeInfo(ComputerSystemType::ATARI800, L"ATARI800", L"Atari 800", L"Atari800");
    static const auto ATARI5200 = ComputerSystemTypeInfo(ComputerSystemType::ATARI5200, L"ATARI5200", L"Atari 5200", L"Atari5200");
    static const auto C64 = ComputerSystemTypeInfo(ComputerSystemType::C64, L"C64", L"C64", L"C64");
    static const auto ORIC = ComputerSystemTypeInfo(ComputerSystemType::ORIC, L"ORIC", L"Oric", L"Oric");

    switch (type) {
    case ComputerSystemType::UNKNOWN:
        return &UNKNOWN;
    case ComputerSystemType::ATARI800:
        return &ATARI800;
    case ComputerSystemType::ATARI5200:
        return &ATARI5200;
    case ComputerSystemType::C64:
        return &C64;
    case ComputerSystemType::ORIC:
        return &ORIC;
    }
    throw std::runtime_error("Unknown computer system type");
}

gsl::not_null<const ComputerSystem*> ComputerSystemFactory::GetComputerSystem(ComputerSystemType type) const {
   switch (type) {
   case ComputerSystemType::UNKNOWN:
       return unknown.get();
    case ComputerSystemType::ATARI800:
        return atari800.get();
    case ComputerSystemType::ATARI5200:
        return atari5200.get();
    case ComputerSystemType::C64:
        return c64.get();
    case ComputerSystemType::ORIC:
        return oric.get();
    default:
        throw std::runtime_error("Invalid computer system type");
    }
}
