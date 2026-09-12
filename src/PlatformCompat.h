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
#include <iconv.h>
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
// Uses iconv (POSIX) rather than mbsrtowcs, so decoding doesn't depend on
// the process locale being set to a UTF-8 one. "WCHAR_T" is glibc's
// pseudo-charset for the platform's native wchar_t encoding, so this
// doesn't need to assume UTF-32 itself.
inline std::wstring Utf8ToWStringCompat(std::string_view str) {
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
