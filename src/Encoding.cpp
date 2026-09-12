#include "Encoding.h"
#include "Syntax.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <stdexcept>
#include <vector>

EncodingInfo EncodingFactory::GetInfo(wstring_view key) {
    static std::map<wstring_view, Encoding> lookup = {
        { L"UNKNOWN", Encoding::UNKNOWN },
        { L"ASCII", Encoding::ASCII },
        { L"BINARY", Encoding::BINARY },
        { L"ATASCII", Encoding::ATASCII },
        { L"UTF8", Encoding::UTF8 },

    };

    Encoding Encoding = Encoding::UNKNOWN;
    try {
        Encoding = lookup.at(key);
    }
    catch (const std::out_of_range&) {}

    return GetInfo(Encoding);
}

EncodingInfo EncodingFactory::GetInfo(const Encoding encoding) {
#ifdef _WIN32
    constexpr auto NEWLINE = L"\r\n";
#else
    constexpr auto NEWLINE = L"\n";
#endif // _WIN32

    if (encoding == Encoding::ASCII) {
        return EncodingInfo(Encoding::ASCII, L"ASCII", L"ASCII", NEWLINE);
    }
    else if (encoding == Encoding::ATASCII) {
        return EncodingInfo(Encoding::ATASCII, L"ATASCII", L"ATASCII", L"\u009b");
    }
    else if (encoding == Encoding::BINARY) {
        return EncodingInfo(Encoding::BINARY, L"BINARY", L"Binary", L"");
    }
    else if (encoding == Encoding::UTF8) {
        return EncodingInfo(Encoding::UTF8, L"UTF8", L"UTF-8", NEWLINE);
    }
    return EncodingInfo(Encoding::UNKNOWN, L"UNKNOWN", L"Unknown", L"");
}


int EncodingFactory::GetIndex(const Encoding encoding, const std::vector<Encoding> encodings, int defaultIndex) {
    auto it = std::find(encodings.begin(), encodings.end(), encoding);
    if (it == encodings.end()) {
        return 0;
    }
    return std::distance(encodings.begin(), it);
}