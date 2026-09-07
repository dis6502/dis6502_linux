#pragma once
#include "ByteArray.h"
#include "Syntax.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <vector>
#ifndef _WIN32
#include <limits.h>
#endif 

class FileIO final
{
public:
    using FILE_SIZE = uintmax_t;
    using FILE_OFFSET = uintmax_t;


#ifdef _WIN32
    static constexpr size_t FILE_PATH_SIZE = _MAX_PATH;
    static constexpr size_t FOLDER_PATH_SIZE = _MAX_PATH;
    static const char FILE_SEPARATOR_CHAR = '\\';
#else
    static constexpr size_t FILE_PATH_SIZE = PATH_MAX;
    static constexpr size_t FOLDER_PATH_SIZE = PATH_MAX;
    static const char FILE_SEPARATOR_CHAR = '/';
#endif
    using FILE_PATH = wchar_t[FILE_PATH_SIZE];
    using FOLDER_PATH = wchar_t[FOLDER_PATH_SIZE];


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

    static std::filesystem::path ToPath(wstring_view filePath);

};
