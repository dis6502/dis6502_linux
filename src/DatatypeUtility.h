#pragma once

#include "Syntax.h"

#include "Byte.h"
#include "Word.h"

class DatatypeUtility {
public:
	static wstring ByteToHexString(const byte value, bool withPrefix);
	static wstring WordToHexString(const word value, bool withPrefix);
    static wstring SizeToHexString(const size_t value, bool withPrefix);
    static wstring UnsignedLongHexString(const unsigned long value, bool withPrefix);
	static wstring ByteArrayToHexString(const byte* valueArray, size_t inSize, bool withPrefix);

    // The method accept decimal and hexa-decimal (prefix 0x) input
	static bool ByteFromString(byte& value, wstring_view stringValue);
	static bool WordFromString(word& value, wstring_view stringValue);
    static bool SizeFromString(size_t& value, wstring_view stringValue);
    static bool UnsignedLongFromString(unsigned long& value, wstring_view stringValue);

    // The method allocates the result on the heap. This means you have to delete it afterwards.
    // In case of an error, false is returned and the valueArray is initialized to nullptr
	static bool ByteArrayFromHexString(byte*& valueArray, size_t& outSize, wstring_view stringValue);

	static void ClearByteArray(byte valueArray[], size_t size);
	static void FillByteArray(byte valueArray[], size_t size, byte value);
private:
	static const wstring_view HEX_CHARACTERS;
};
