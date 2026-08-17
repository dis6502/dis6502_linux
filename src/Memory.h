#pragma once

#include "Syntax.h"

// Model of an 8-bit memory with 16 bit address space and little endian byte order.
class Memory final {

public:
    using size = unsigned int;
    static const size MAX_SIZE = 0x10000;
    using offset = unsigned short;
    static const size MAX_OFFSET = 0xffff;

    using byte = unsigned char;              // one byte, 8 bits
    using word = unsigned short int;         // two bytes, 16 bits
    using address = unsigned short int;      // two bytes, 16 bits
    using address_offset = signed short int; // two bytes, 16 bits, signed

    using dword = unsigned long;    // four bytes, 32 bits

    static constexpr word to_word(byte low, byte high) { word result = high;  result = low | (result) << 8; return result; };
    static constexpr address to_address(byte low, byte high) { address result = high;  result = low | (result) << 8; return result; };
    static constexpr byte to_low_byte(word word) { return (word & 0xFF); };
    static constexpr byte to_high_byte(word word) { return ((word >> 8) & 0xFF); };


    static wstring size_to_hex_string(size size);
    static wstring size_to_string(size size);
    static wstring offset_to_hex_string(offset offset);

    static wstring byte_to_hex_string(byte byte);
    static wstring address_to_hex_string(address address);
    static wstring address_offset_to_string(address_offset address_offset);

};