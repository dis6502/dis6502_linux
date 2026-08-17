#pragma once

#include "Syntax.h"
#include "Byte.h"
#include <sstream>

class AssertionError : public std::runtime_error {


public:
    AssertionError(wstring_view message);

};

class Assert {
public:

    static void BoolEquals(bool actual, bool expected);

    //static void AddressEquals(Memory::address actual, Memory::address expected);

    static wstring ByteToString(byte value);
    static void ByteEquals(byte actual, byte expected);

    static wstring LongToString(long long value);
    static void LongEquals(long long actual, long long expected);

    static void StringEquals(wstring_view actual, wstring_view expected);

    static void Log(const wstringstream& text);
    static void Log(wstring_view text);
    static void LogValue(wstring_view label, wstring_view value);
    static void Log(const std::exception& exception);

    static void PointerEquals(void* actual, void* expected);
    static void PointerNotNull(void* actual);

    static void Fail(const wstringstream& text);
    static void Fail(wstring_view text);
    static void Fail(const std::exception &exception);
};