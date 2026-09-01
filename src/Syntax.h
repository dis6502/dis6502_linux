#pragma once

// Always include this as the first include in the header files.

// #define _CRTDBG_MAP_ALLOC
#ifdef  _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib> 
#endif //  _CRTDBG_MAP_ALLOC

#include <cstring>
#include <memory> // for std::unique_ptr
#include <sstream>
#include <stdexcept>
#include <string>

using string = std::string;
using wstring = std::wstring;
using string_view = std::string_view;
using wstring_view = std::wstring_view;
using wstringstream = std::wstringstream;

namespace dis_k {
    constexpr int SDX_SYMBOL_LEN = 8;
    constexpr unsigned short int NO_DUMP = 0xFFFF;
    constexpr unsigned short int DUMP_NO_SELECTION = 0xFFFF;
}

#define safeExit(MESSAGE) \
{ fprintf(stderr, "ERROR: %s",(MESSAGE)); \
  fprintf(stderr, "Exit at file %s, line %d.",__FILE__, __LINE__); \
  exit(EXIT_FAILURE); \
}

#define safeExitWithExitCode(MESSAGE,EXIT_CODE) \
{ fprintf(stderr, "ERROR: %s",(MESSAGE)); \
  fprintf(stderr, "Exit at file %s, line %d.",__FILE__, __LINE__); \
  exit(EXIT_CODE); \
}

#define safeExitWithClassName(CLASSNAME) \
{ safeExit(String::wstring_to_ansi(String::Format(L"Cannot register class %s", (CLASS_NAME))).c_str()); }
