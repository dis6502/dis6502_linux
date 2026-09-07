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


// ---- wsprintf: old Win32 User32 API, unbounded but in practice capped
// at 1024 wide chars (its historical real-world limit on Windows).
//
// Win32's wsprintf has no narrow/wide distinction for %s - in a wide
// format string, %s always means "wide string", same as %ls. POSIX's
// vswprintf disagrees: per the standard, a bare %s in a WIDE format
// string takes a narrow char* (converted via the current locale) - %ls
// is the one that takes wchar_t*. Every call site in this codebase
// (ported straight from Windows) passes wchar_t*/wstring::c_str() to
// %s, so forwarding the format string to vswprintf unchanged is
// undefined behavior: vswprintf reads the wchar_t* as if it were a
// char*, and on this platform's little-endian 4-byte wchar_t, that
// hits the zero byte right after the first character - e.g. formatting
// L"Start of code" through a bare %s silently truncates to L"S". This
// rewrites every bare %s (not already %ls/%hs, and not a literal %%)
// to %ls before handing the format string to vswprintf, so callers get
// the same wide-string behavior as real Win32 wsprintf without having
// to touch every call site individually. ----
std::wstring RewriteBareWideStringSpecifiers(const wchar_t* format) {
    std::wstring result;
    for (const wchar_t* p = format; *p; ) {
        if (*p != L'%') { result += *p++; continue; }
        // Copy the '%' itself, then a literal '%%' just copies the
        // second '%' too and moves on - nothing to rewrite.
        result += *p++;
        if (*p == L'%') { result += *p++; continue; }
        // Flags.
        while (*p == L'-' || *p == L'+' || *p == L' ' || *p == L'#' || *p == L'0') { result += *p++; }
        // Width (digits or '*').
        while ((*p >= L'0' && *p <= L'9') || *p == L'*') { result += *p++; }
        // Precision.
        if (*p == L'.') {
            result += *p++;
            while ((*p >= L'0' && *p <= L'9') || *p == L'*') { result += *p++; }
        }
        // Length modifier - if present, this specifier already says
        // exactly what width it wants; leave it alone.
        bool hasLengthModifier = false;
        if (*p == L'h' || *p == L'l' || *p == L'j' || *p == L'z' || *p == L't' || *p == L'L' || *p == L'q') {
            hasLengthModifier = true;
            wchar_t modifier = *p;
            result += *p++;
            if (*p == modifier) { result += *p++; } // hh/ll
        }
        // Conversion character - insert 'l' before a bare 's' only.
        if (*p == L's' && !hasLengthModifier) { result += L'l'; }
        if (*p) { result += *p++; }
    }
    return result;
}