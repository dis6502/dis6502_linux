#pragma once

#include "Syntax.h"
#include <cstdint> // for uintmax_t below - this was previously relying on
                   // transitive inclusion via another header, which
                   // isn't a portable guarantee across g++/libstdc++
                   // versions (compiled fine in one environment, failed
                   // in another with the exact same source)

// TODO: Replace remaining usage of char array by strings
void strclr(char* szString);
void strclr(wchar_t* szString);

template <typename T> T Argument(T value) { return value; }
template <typename T> T const* Argument(std::basic_string<T> const& value) { return value.c_str(); }
template <typename T> T const* Argument(std::string_view const& value) { return string(value).c_str(); }
template <typename ... Args> void SPrintf(char* buffer, char const* const format, Args const& ... args) { sprintf(buffer, format, Argument(args) ...); }

namespace std {

    wstring to_hex_string(uintmax_t value);
    wstring to_byte_hex_string(uintmax_t value);

}

class String {
public:

    static wstring FromCString(const wchar_t* szString);
    static wstring FromCharArray(const char array[], size_t size);
    static void AssertASCII(const wchar_t*);

    static wstring Empty();

    static size_t Length(const wchar_t* szString);

    static int Compare(const wchar_t* a, const wchar_t* b, size_t count);
    static const wchar_t* FindFirst(const wchar_t* szString, const wchar_t* szSubString);

    static bool Equals(wstring_view a, wstring_view b);
    static bool EqualsIgnoreCase(wstring_view a, wstring_view b);
    static bool StartsWith(wstring_view s, wstring_view suffix);
    static bool EndsWith(wstring_view s, wstring_view suffix);

    // Trim leading and trailing whitespaces
    static wstring Trim(wstring_view s);
    static wstring ToLower(wstring_view s); // Only works correctly for ASCII strings

    static void FindAndReplaceAll(wstring& data, wstring_view toSearch, wstring_view replaceStr);

    // Format a string pattern of the form "Foo {0} bar {1}..."
    static wstring Format(wstring_view pattern, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");

    // Workaround for using sprintf
    static const size_t BUFFER_SIZE = 1000;
    static wchar_t* szBuffer;
    static wstring Format();

    // Transition to Unicode
    static string wstring_to_ansi(wstring_view s);
    static wstring ansi_to_wstring(string_view s);

    // convert UTF-8 string to wstring
    static wstring utf8_to_wstring(string_view str);

    // convert wstring to UTF-8 string
    static string wstring_to_utf8(wstring_view str);
};