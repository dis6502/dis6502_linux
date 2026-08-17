#include "Assertions.h"
#include <sstream>
#include "Debug.h"
#include "PlatformCompat.h"
#include "Strings.h"
#ifdef _WIN32
#include <Windows.h>
#endif

wstring ToString(void* pointer) {
    constexpr size_t BUFFER_SIZE = 32;
    wchar_t buffer[BUFFER_SIZE];
    wsprintf(buffer, L"%p", pointer);
    return wstring(buffer);
}

AssertionError::AssertionError(wstring_view message) :
    std::runtime_error(String::wstring_to_utf8(message)) {

}

void Assert::BoolEquals(bool actual, bool expected) {
    if (actual != expected) {
        LogValue(L"Actual Value  ", std::to_wstring(actual));
        LogValue(L"Expected Value", std::to_wstring(expected));
        Fail(L"Bool values not equal");
    }
}

wstring Assert::ByteToString(byte value) {
    return (wstringstream() << value << " (0x" << std::hex << value << ", '" << (char)value << "')").str();
}


void Assert::ByteEquals(byte actual, byte expected) {
    if (actual != expected) {
        LogValue(L"Actual Value  ", ByteToString(actual));
        LogValue(L"Expected Value", ByteToString(expected));
        Fail(L"Byte values not equal");
    }
}

wstring Assert::LongToString(long long value) {
    return (wstringstream() << value << " (0x" << std::hex << value << ")").str();
}

void Assert::LongEquals(long long actual, long long expected) {
    if (actual != expected) {
        LogValue(L"Actual Value  ", LongToString(actual));
        LogValue(L"Expected Value", LongToString(expected));
        Fail(L"Long values not equal");
    }
}

//void Assert::AddressEquals(Memory::address actual, Memory::address expected) {
//    if (actual != expected) {
//        Fail("Not equal");
//    }
//}

void Assert::StringEquals(wstring_view actual, wstring_view expected) {
    if (actual != expected) {
        LogValue(L"Actual Value  ", actual);
        LogValue(L"Expected Value", expected);
        Fail(L"String values not equal");
    }
}

void Assert::PointerEquals(void* actual, void* expected) {
    if (actual != expected) {
        LogValue(L"Actual Value  ", ToString(actual));
        LogValue(L"Expected Value", ToString(expected));
        Fail(L"Pointer values not equal");
    }
}

void Assert::PointerNotNull(void* actual) {
    if (actual == 0) {
        LogValue(L"Actual Value  ", ToString(actual));
        LogValue(L"Expected Value", L"nullptr");
        Fail(L"Pointer values not equal");
    }
}

void Assert::Log(const wstringstream& text) {
    Debug::Log(text.str());
}

void Assert::Log(wstring_view text) {
    Debug::Log(text);
}

void Assert::LogValue(wstring_view label, wstring_view value) {
    Debug::LogValue(label, value);
}

void Assert::Log(const std::exception& exception) {
    Log(String::utf8_to_wstring(exception.what()));
}

void Assert::Fail(const wstringstream& text) {
    Fail(text.str());
}

void Assert::Fail(wstring_view text) {
    Log(text);
    throw AssertionError(text);
}

void Assert::Fail(const std::exception& exception) {
    Fail(String::utf8_to_wstring(exception.what()));
}