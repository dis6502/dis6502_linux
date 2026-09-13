// wcscasecmp() lives in <wchar.h> and is gated behind POSIX.1-2008
// visibility, which -std=c++20 (strict mode) doesn't turn on by default.
// This must be defined before the first system header is pulled in
// (including transitively via <string>), or the header guard in
// wchar.h locks in the lower visibility for the rest of the translation
// unit.
#ifndef _WIN32
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#endif

#include "StringUtility.h"
#include "Syntax.h"
#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstdint>
#include <format>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string.h>
#include <vector>
#include <wchar.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <iconv.h>
#endif

#include "Assertions.h"

wchar_t* String::szBuffer = new wchar_t[BUFFER_SIZE];

#ifndef _WIN32
// ---- UTF-8 <-> wstring conversion ----
// Uses iconv (POSIX) rather than mbsrtowcs, so decoding doesn't depend on
// the process locale being set to a UTF-8 one. "WCHAR_T" is glibc's
// pseudo-charset for the platform's native wchar_t encoding, so this
// doesn't need to assume UTF-32 itself.
static std::wstring Utf8ToWStringCompat(std::string_view str) {
    iconv_t cd = iconv_open("WCHAR_T", "UTF-8");
    if (cd == (iconv_t)-1) { return {}; }

    std::wstring result(str.size(), L'\0'); // worst case: 1 wchar_t per input byte
    char* inBuf = const_cast<char*>(str.data());
    size_t inBytes = str.size();
    char* outBuf = reinterpret_cast<char*>(result.data());
    size_t outBytes = result.size() * sizeof(wchar_t);

    iconv(cd, inBytes ? &inBuf : nullptr, &inBytes, &outBuf, &outBytes);
    iconv_close(cd);

    result.resize(result.size() - outBytes / sizeof(wchar_t));
    return result;
}

static std::string WStringToUtf8Compat(std::wstring_view str) {
    std::string result;
    result.reserve(str.size());
    for (wchar_t wc : str) {
        char32_t cp = static_cast<char32_t>(wc);
        if (cp <= 0x7F) {
            result.push_back(static_cast<char>(cp));
        }
        else if (cp <= 0x7FF) {
            result.push_back(static_cast<char>(0xC0 | (cp >> 6)));
            result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF) {
            result.push_back(static_cast<char>(0xE0 | (cp >> 12)));
            result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else {
            result.push_back(static_cast<char>(0xF0 | (cp >> 18)));
            result.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
    }
    return result;
}

static int _wcsicmp(const wchar_t* a, const wchar_t* b) {
    return wcscasecmp(a, b);
}
#endif


void strclr(char* szString) {
    if (szString == nullptr) {
        throw std::runtime_error("String must not be nullptr");
    }
    szString[0] = 0;
};

void strclr(wchar_t* szString) {
    if (szString == nullptr) {
        throw std::runtime_error("String must not be nullptr");
    }
    szString[0] = 0;
};

wstring std::to_hex_string(uintmax_t value) {

    return std::format(L"{:#x}", value);
}

wstring std::to_byte_hex_string(uintmax_t value) {
    if (value < 0x10) {
        return (wostringstream() << std::hex << "0x0" << value).str();

    }
    if (value < 0x100) {
        return (wostringstream() << std::hex << "0x" << (value)).str();
    }
    return L"value_out_of_byte_range";
}

wstring String::FromCString(const wchar_t* szString) {
    if (szString == nullptr) {
        return Empty();
    }
    return wstring(szString);
}

wstring String::FromCharArray(const char array[], size_t size) {
    if (array == nullptr) {
        return Empty();
    }
    std::vector<wchar_t> buffer;
    buffer.reserve(size + 1);
    for (size_t i = 0; i < size; i++) { buffer[i] = array[i]; };
    buffer[size] = 0;
    return wstring(buffer.data());
}
void String::AssertASCII(const wchar_t* szString) {
    for (size_t i = 0; i < Length(szString); i++) {
        auto c = szString[i];
        if (c < 32 || c>127) {
            throw std::runtime_error("Invalid character.");
        }
    }
}

wstring String::Empty() {
    return wstring(L"");
}

size_t String::Length(const wchar_t* szString) {
    if (szString == nullptr) {
        return 0;
    }
    return wcslen(szString);
}
int String::Compare(const wchar_t* a, const wchar_t* b, size_t count) {
    return wcsncmp(a, b, count);
}
const wchar_t* String::FindFirst(const wchar_t* szString, const wchar_t* szSubString) {
    return wcsstr(szString, szSubString);
}
bool String::Equals(wstring_view a, wstring_view b) {
    return (a.compare(b) == 0);
}

bool String::EqualsIgnoreCase(wstring_view a, wstring_view b) {
    return (_wcsicmp(wstring(a).c_str(), wstring(b).c_str()) == 0);  // ##c_str() OK
}

bool String::StartsWith(wstring_view s, wstring_view prefix) {
    if (s.length() >= prefix.length()) {
        return (0 == s.compare(0, prefix.length(), prefix));
    }
    return false;
}

bool String::EndsWith(wstring_view s, wstring_view suffix) {
    if (s.length() >= suffix.length()) {
        return (0 == s.compare(s.length() - suffix.length(), suffix.length(), suffix));
    }
    return false;
}

wstring String::Trim(wstring_view s) {
    static const wchar_t* whitespaces = L" \n\r\t\f\v";
    const auto start = s.find_first_not_of(whitespaces);
    if (start == wstring::npos) {
        return L"";
    }
    const auto end = s.find_last_not_of(whitespaces);
    const wstring_view text = s.substr(start, end - start + 1);

    return wstring(text);
}

wstring String::ToLower(wstring_view s) {
    auto result = wstring(s);
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); }
    );
    return result;
}

void String::FindAndReplaceAll(wstring& data, wstring_view toSearch, wstring_view replaceStr) {
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while (pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = data.find(toSearch, pos + replaceStr.size());
    }
}

wstring String::Format(wstring_view pattern, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    wstring result = wstring(pattern);
    FindAndReplaceAll(result, L"{0}", v0);
    FindAndReplaceAll(result, L"{1}", v1);
    FindAndReplaceAll(result, L"{2}", v2);
    FindAndReplaceAll(result, L"{3}", v3);
    FindAndReplaceAll(result, L"{4}", v4);
    FindAndReplaceAll(result, L"{5}", v5);
    FindAndReplaceAll(result, L"{6}", v6);
    FindAndReplaceAll(result, L"{7}", v7);
    FindAndReplaceAll(result, L"{8}", v8);
    FindAndReplaceAll(result, L"{9}", v9);
    return result;
}
string String::wstring_to_ansi(wstring_view s) {
    return string(s.cbegin(), s.cend());
}

wstring String::ansi_to_wstring(string_view s) {
    return wstring(s.cbegin(), s.cend());
}

wstring String::utf8_to_wstring(string_view str) {
#ifdef _WIN32
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size_needed);
    return result;
#else
    return Utf8ToWStringCompat(str);
#endif
}

string String::wstring_to_utf8(wstring_view str) {
#ifdef _WIN32
    if (str.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0, NULL, NULL);
    string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, str.data(), (int)str.size(), &result[0], size_needed, NULL, NULL);
    return result;
#else
    return WStringToUtf8Compat(str);
#endif
}

void String::FormatV(wchar_t* buffer, wchar_t const* format, ...) {
    // wsprintfW itself has no caller-supplied size and an undocumented ~1024-wchar_t internal
    // cap; this mirrors that instead of pretending to be safe with an unknown buffer size.
    constexpr size_t MAX_LEGACY_FORMAT_LENGTH = 1024;

    va_list args;
    va_start(args, format);
    vswprintf(buffer, MAX_LEGACY_FORMAT_LENGTH, format, args);
    va_end(args);
}
