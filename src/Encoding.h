#pragma once


#include "Syntax.h"
#include <fcntl.h>	// For file open flags
#include <vector>

enum class Encoding {
    UNKNOWN,
    ASCII,
    ATASCII,
    BINARY,
    UTF8,
};

struct EncodingInfo {
    const Encoding encoding;
    const wstring key;
    const wstring text;
    const wstring newline;
    const int flags;

    EncodingInfo(const Encoding encoding, const wstring_view key, const wstring_view text, const wstring_view newline, int flags) :
        encoding{ encoding }, key{ key }, text{ text }, newline{ newline }, flags{ flags } {
    }
};

class EncodingFactory {
public:
    static EncodingInfo GetInfo(const wstring_view key);
    static EncodingInfo GetInfo(const Encoding encoding);
    static int GetIndex(const Encoding encoding, const std::vector<Encoding> encodings, int defaultIndex);

};
