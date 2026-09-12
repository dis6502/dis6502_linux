#pragma once
// Linux/POSIX replacements for the small set of Windows-CRT-only calls
// used by the (originally Win32) core engine. Windows builds should
// never include this header (guarded so it's a no-op there).

#ifndef WIN32

#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <string>
#include <strings.h>

#define _TRUNCATE ((size_t)-1)

inline int wcsncpy_s(wchar_t* dest, size_t destSize, const wchar_t* src, size_t count) {
    size_t n = (count == _TRUNCATE || count >= destSize) ? destSize - 1 : count;
    std::wcsncpy(dest, src, n);
    dest[n] = L'\0';
    return 0;
}

// ---- UTF-8 <-> wstring conversion (moved up front: several shims below depend on it) ----
inline std::wstring Utf8ToWStringCompat(std::string_view str) {
    std::wstring result;
    result.reserve(str.size());
    size_t i = 0;
    while (i < str.size()) {
        unsigned char c0 = static_cast<unsigned char>(str[i]);
        char32_t cp = 0;
        size_t extra = 0;
        if ((c0 & 0x80) == 0) { cp = c0; extra = 0; }
        else if ((c0 & 0xE0) == 0xC0) { cp = c0 & 0x1F; extra = 1; }
        else if ((c0 & 0xF0) == 0xE0) { cp = c0 & 0x0F; extra = 2; }
        else if ((c0 & 0xF8) == 0xF0) { cp = c0 & 0x07; extra = 3; }
        else { cp = 0xFFFD; extra = 0; } // invalid lead byte
        i++;
        for (size_t k = 0; k < extra && i < str.size(); k++, i++) {
            unsigned char cx = static_cast<unsigned char>(str[i]);
            if ((cx & 0xC0) != 0x80) { cp = 0xFFFD; break; }
            cp = (cp << 6) | (cx & 0x3F);
        }
        result.push_back(static_cast<wchar_t>(cp));
    }
    return result;
}

inline std::string WStringToUtf8Compat(std::wstring_view str) {
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


// ---- _MAX_PATH: Windows CRT max-path-length constant ----
#include <climits>
#ifndef _MAX_PATH
#define _MAX_PATH PATH_MAX
#endif

// ---- case-insensitive string compare ----
int _stricmp(const char* a, const char* b);
int _wcsicmp(const wchar_t* a, const wchar_t* b);


// ---- strncpy_s: MSVC "secure CRT" string copy. Two overloads are used
// in this codebase: the fixed-array-destination template form, and the
// explicit (dest, destSize, src, count) form with _TRUNCATE. ----

template <size_t N>
inline int strncpy_s(char(&dest)[N], const char* src, size_t count) {
    size_t n = (count < N - 1) ? count : N - 1;
    std::strncpy(dest, src, n);
    dest[n] = '\0';
    return 0;
}

inline int strncpy_s(char* dest, size_t destSize, const char* src, size_t count) {
    size_t n = (count == _TRUNCATE || count >= destSize) ? destSize - 1 : count;
    std::strncpy(dest, src, n);
    dest[n] = '\0';
    return 0;
}


#endif // !_WIN32
