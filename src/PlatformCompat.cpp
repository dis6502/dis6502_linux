// wcscasecmp() lives in <wchar.h>, not <strings.h>, and both it and
// strcasecmp() are gated behind POSIX.1-2008 visibility, which
// -std=c++20 (strict mode) doesn't turn on by default. This must be
// defined before the first system header is pulled in (including
// transitively via PlatformCompat.h's own <cwchar>), or the header
// guard in wchar.h locks in the lower visibility for the rest of the
// translation unit.
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include "PlatformCompat.h"

#include <strings.h>
#include <wchar.h>

// Used in XML.h
int _stricmp(const char* a, const char* b) {
    return strcasecmp(a, b);
}
int _wcsicmp(const wchar_t* a, const wchar_t* b) {
    return wcscasecmp(a, b);
}
