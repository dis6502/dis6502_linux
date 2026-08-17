#include "Memory.h"

#include "String.h"
#include "DatatypeUtility.h"

wstring Memory::size_to_hex_string(Memory::size size) {
    if (size < Memory::MAX_SIZE) {
        return DatatypeUtility::WordToHexString(size, false);
    }
    else if (size == Memory::MAX_SIZE) {
        return L"10000";
    };
    throw std::runtime_error(String::wstring_to_ansi(String::Format(L"Size {0} exceeds maxium size of {1}", std::to_hex_string(size), std::to_hex_string(MAX_SIZE))));
}

wstring Memory::size_to_string(Memory::size size) {
    return std::to_wstring(size);
}

wstring Memory::offset_to_hex_string(Memory::offset offset) {
    if (offset <= Memory::MAX_OFFSET) {
        return DatatypeUtility::WordToHexString(offset, false);
    };
    throw std::runtime_error(String::wstring_to_ansi(String::Format(L"Offset {0} exceeds maxium size of {1}", std::to_hex_string(offset), std::to_hex_string(MAX_OFFSET))));
}

wstring Memory::byte_to_hex_string(Memory::byte byte) {
    return DatatypeUtility::ByteToHexString(byte, false);
}

wstring Memory::address_to_hex_string(Memory::address address) {
    return DatatypeUtility::WordToHexString(address, false);
}

wstring Memory::address_offset_to_string(Memory::address_offset address_offset) {
    if (address_offset == 0) {
        return L"";
    }
    if (address_offset > 0) {
        return L"+" + std::to_wstring(address_offset);
    }
    else {
        return std::to_wstring(address_offset); // "-" is prefix already

    }
}
