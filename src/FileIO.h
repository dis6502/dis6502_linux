#pragma once

#include "Syntax.h"
#include "IO.h"
#include "PlatformCompat.h"
#include <cstdint>
#include <vector>
#include "ByteArray.h"

// Include standard IO flags
#include <fstream>

class FileIO final
{
public:
    using FILE_SIZE = uintmax_t;
    using FILE_OFFSET = uintmax_t;

    static constexpr size_t FILE_PATH_SIZE = _MAX_PATH;
    static constexpr size_t FOLDER_PATH_SIZE = _MAX_PATH;

    using FILE_PATH = wchar_t[FILE_PATH_SIZE];
    using FOLDER_PATH = wchar_t[FOLDER_PATH_SIZE];

#ifdef _WIN32
    static const char FILE_SEPARATOR_CHAR = '\\';
#else
    static const char FILE_SEPARATOR_CHAR = '/';
#endif
    static const wstring FILE_SEPARATOR;
    static const wstring EMPTY_FILE_PATH;

    static bool FileExists(wstring_view filePath);
    static bool IsFileReadOnly(wstring_view filePath);
    static FILE_SIZE GetFileSize(wstring_view filePath); // Throws IOException
    static ByteArray ReadByteArray(wstring_view filePath); // Throws IOException
    static wstring ReadString(wstring_view filePath); // Throws IOException
    static std::vector<wstring> ReadStrings(wstring_view filePath); // Throws IOException


    // TODO Replace
    static FILE* OpenFile(wstring_view filePath, wstring_view mode);
    static void CloseFile(FILE* fd);

    static void SetCurrentWorkingDirectory(wstring_view folderPath); // Throws IOException
};
