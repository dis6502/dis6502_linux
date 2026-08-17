#include <ctype.h>

#include "DatatypeUtility.h"
#include <ios>

const wstring_view DatatypeUtility::HEX_CHARACTERS = { L"0123456789ABCDEF" };

wstring DatatypeUtility::ByteToHexString(const byte value, bool withPrefix) {
    return ByteArrayToHexString(&value, 1, withPrefix);
}

wstring DatatypeUtility::WordToHexString(const word value, bool withPrefix) {
    // Assume little endian byte order for word.
    byte buffer[2]{};
    buffer[0] = (value >> 8) & 0xFF;
    buffer[1] = value & 0xFF;

    return ByteArrayToHexString(buffer, 2, withPrefix);
}

wstring DatatypeUtility::SizeToHexString(const size_t value, bool withPrefix) {
    return UnsignedLongHexString(value, withPrefix);
}

wstring DatatypeUtility::UnsignedLongHexString(const unsigned long value, bool withPrefix) {
    wstringstream ws;
    ws.imbue(std::locale("C")); // Disable locale settings like thousands separator
    ws << std::hex;
    if (withPrefix) {
        ws << std::showbase;
    }
    ws << value;
    return ws.str();
}

wstring DatatypeUtility::ByteArrayToHexString(const byte* valueArray, size_t size, bool withPrefix) {
    wstring outBuffer = withPrefix ? L"0x" : L"";

    for (size_t i = 0; i < size; ++i) {
        const byte value = valueArray[i];
        outBuffer += HEX_CHARACTERS.at((value >> 4) & 0xF);
        outBuffer += HEX_CHARACTERS.at(value & 0xF);
    }

    return outBuffer;
}

bool DatatypeUtility::ByteFromString(byte& value, wstring_view stringValue) {
    word wordValue = 0;
    if (WordFromString(wordValue, stringValue)) {
        if (wordValue <= 255) {
            value = static_cast<byte>(wordValue);
            return true;
        }
    }

    return false;
}

bool DatatypeUtility::WordFromString(word& value, wstring_view stringValue) {
    unsigned long longValue = 0;
    if (UnsignedLongFromString(longValue, stringValue)) {
        if (longValue <= 65535) {
            value = static_cast<word>(longValue);
            return true;
        }
    }

    return false;
}

bool DatatypeUtility::SizeFromString(size_t& value, wstring_view stringValue) {
    unsigned long longValue = 0;
    if (UnsignedLongFromString(longValue, stringValue)) {
        value = static_cast<size_t>(longValue);
        return true;
    }

    return false;
}

bool DatatypeUtility::UnsignedLongFromString(unsigned long& value, wstring_view stringValue) {
    const int length = stringValue.length();
    if (length < 1) {
        return false;
    }

    try {
        if (stringValue.starts_with(L"0x") || stringValue.starts_with(L"0X")) {
            value = std::stoul(wstring(stringValue), nullptr, 16);
        }
        else {
            value = std::stoul(wstring(stringValue), nullptr, 10);
        }

        return true;
    }
    catch (const std::invalid_argument&) {
        return false;
    }
}

bool DatatypeUtility::ByteArrayFromHexString(byte*& valueArray, size_t& size, wstring_view stringValue) {
    valueArray = nullptr;
    int length = 0;
    try {
        length = stringValue.length();
    }
    catch (...) {
        safeExit();
    }
    if (length < 2) {
        return false;
    }

    int j = 0;
    if (stringValue.at(0) == L'0' && stringValue.at(1) == L'x') {
        j = 2;
        length -= j;
    }
    else {
        return false;
    }

    if (length & 0x1) {
        return false;
    }

    size = length / 2;

    auto buffer = new byte[size];
    wchar_t convertBuffer[3]{};

    for (size_t i = 0; i < size; ++i) {
        convertBuffer[0] = stringValue.at(j++);
        convertBuffer[1] = stringValue.at(j++);
        convertBuffer[2] = 0;

        size_t pos = 0;
        try {
            auto value = std::stol(convertBuffer, &pos, 16);

            if (value < 0 || value > 255) {
                delete[] buffer;
                return false;
            }

            buffer[i] = static_cast<byte>(value);
        }
        catch (const std::invalid_argument&) {
            delete[] buffer;
            return false;
        }
    }
    valueArray = buffer;
    return true;
}

void DatatypeUtility::ClearByteArray(byte arrayValue[], size_t size) {
    FillByteArray(arrayValue, size, 0);
}

void DatatypeUtility::FillByteArray(byte arrayValue[], size_t size, byte value) {
    std::fill_n(arrayValue, size, value);
}
