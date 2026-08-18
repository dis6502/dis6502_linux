#pragma once
// Linux/POSIX replacements for the small set of Windows-CRT-only calls
// used by the (originally Win32) core engine. Windows builds should
// never include this header (guarded so it's a no-op there).

#ifndef WIN32

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <string>

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

// ---- AddFontResource/RemoveFontResource: GDI's temporary font-file
// registration mechanism, used originally to install a bundled .fon
// bitmap font for rendering the Atari/C64 character set. No portable
// meaning outside GDI - real font loading belongs to the UI layer
// (ImGui uses its own font-atlas system entirely). Stubbed as no-ops so
// ComputerSystem still compiles/runs headlessly. ----
inline int AddFontResource(const wchar_t* /*fontFilePath*/) { return 1; } // pretend success
inline int RemoveFontResource(const wchar_t* /*fontFilePath*/) { return 1; }

// ---- Win32 base integer typedefs/macros used directly in engine code ----
using BYTE = unsigned char;
using WORD = unsigned short;
using DWORD = unsigned long;
using BOOL = int;
using UINT = unsigned int;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

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
#include <cstdarg>
#include <string>
inline std::wstring RewriteBareWideStringSpecifiers(const wchar_t* format) {
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

inline int wsprintf(wchar_t* buffer, const wchar_t* format, ...) {
    std::wstring fixedFormat = RewriteBareWideStringSpecifiers(format);
    va_list args;
    va_start(args, format);
    int result = std::vswprintf(buffer, 1024, fixedFormat.c_str(), args);
    va_end(args);
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

// ---- opaque handle stand-ins for GDI resource types that "core" data
// structures store (but never actually dereference/draw with) — e.g.
// ComputerFont/Workspace hold a font handle as a settings value. The real
// UI layer's own font handles are wired in separately later. ----
using HFONT = void*;
constexpr HFONT NULL_HFONT_COMPAT = nullptr;

// ---- low-level POSIX file-descriptor I/O, replacing <io.h>'s
// _wsopen/_close/_write and their Windows-only flag names. There's no
// text/binary distinction or file-sharing-mode concept on POSIX, so
// those flags become no-ops. ----
#include <fcntl.h>
#include <unistd.h>

constexpr int _O_CREAT = O_CREAT;
constexpr int _O_TRUNC = O_TRUNC;
constexpr int _O_WRONLY = O_WRONLY;
constexpr int _O_TEXT = 0;
constexpr int _O_BINARY = 0;
constexpr int _O_U8TEXT = 0;
constexpr int _SH_DENYNO = 0;   // no POSIX equivalent needed for this single-process tool
constexpr int _S_IWRITE = 0644; // used here as the create-mode, not a share flag

inline int _wsopen(const wchar_t* path, int oflag, int /*shflag*/, int pmode) {
    std::string narrowPath = WStringToUtf8Compat(path);
    return open(narrowPath.c_str(), oflag, pmode);
}

inline int _close(int fd) { return close(fd); }
inline long _write(int fd, const void* buffer, unsigned int count) {
    return static_cast<long>(write(fd, buffer, count));
}

// ---- wide-char file I/O (Windows CRT extensions) ----
using errno_t = int;

inline FILE* _wfopen(const wchar_t* path, const wchar_t* mode) {
    // Convert both the path and mode from wide chars to UTF-8/narrow,
    // since POSIX fopen() takes narrow paths (glibc treats them as
    // whatever the current locale/filesystem encoding is, typically UTF-8).
    std::string narrowPath = WStringToUtf8Compat(path);
    std::string narrowMode(mode, mode + wcslen(mode)); // mode is always ASCII ("r", "rb", "w+", etc.)
    return fopen(narrowPath.c_str(), narrowMode.c_str());
}

inline errno_t _get_errno(errno_t* out) { *out = errno; return 0; }

inline wchar_t* _wcserror(errno_t errnum) {
    static thread_local wchar_t buf[256];
    const char* msg = strerror(errnum);
    size_t n = std::mbstowcs(buf, msg, 255);
    buf[n == static_cast<size_t>(-1) ? 0 : n] = L'\0';
    return buf;
}

// ---- OutputDebugString: just log to stderr on Linux ----
inline void OutputDebugString(const wchar_t* text) {
    std::fwprintf(stderr, L"%ls", text);
}

// ---- _memccpy: copy from src to dest until character c is copied,
// or n bytes have been copied, matching MSVC's _memccpy semantics.
// Returns pointer just past the copied c in dest, or NULL if not found. ----
inline void* _memccpy(void* dest, const void* src, int c, size_t n) {
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
        if (s[i] == static_cast<unsigned char>(c)) {
            return d + i + 1;
        }
    }
    return nullptr;
}

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
