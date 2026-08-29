#include "Encoding.h"
#include <algorithm>

#include <map>

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
	const int O_TEXT=1;
	const int O_BINARY=2;
	const int O_UTF8=3;
	
    if (encoding == Encoding::ASCII) {
        return EncodingInfo(Encoding::ASCII, L"ASCII", L"ASCII", L"\n", O_TEXT);
    }
    else if (encoding == Encoding::ATASCII) {
        return EncodingInfo(Encoding::ATASCII, L"ATASCII", L"ATASCII", L"\u009b", O_BINARY);
    }
    else if (encoding == Encoding::BINARY) {
        return EncodingInfo(Encoding::BINARY, L"BINARY", L"Binary", L"\n", O_BINARY);
    }
    else if (encoding == Encoding::UTF8) {
        return EncodingInfo(Encoding::UTF8, L"UTF8", L"UTF-8", L"\n", O_UTF8);
    }
    return EncodingInfo(Encoding::UNKNOWN, L"UNKNOWN", L"Unknown", L"", 0);
}


int EncodingFactory::GetIndex(const Encoding encoding, const std::vector<Encoding> encodings, int defaultIndex) {
    auto it = std::find(encodings.begin(), encodings.end(), encoding);
    if (it == encodings.end()) {
        return 0;
    }
    return std::distance(encodings.begin(), it);
}