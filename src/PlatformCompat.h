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

// ---- _MAX_PATH: Windows CRT max-path-length constant ----
#include <climits>
#ifndef _MAX_PATH
#define _MAX_PATH PATH_MAX
#endif

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
