#pragma once

#include "Syntax.h"
#include <span>

enum class FolderType;


enum class FileType {
    UNKNOWN_FILE,
    RAW_FILE,
    EXECUTABLE_FILE,
    ROM_IMAGE_FILE,
    CASSETTE_IMAGE_FILE,
    DISK_IMAGE_EXECUTABLE_FILE,
    DISK_IMAGE_BOOT_SECTORS,
    DISK_IMAGE_SECTORS,
    WORKSPACE_FILE,
    EQUATES_FILE,
    PROFILE_FILE,
    DISASSEMBLY_FILE
};

class FileTypeFactory;

class FileTypeInfo {

public:
    typedef wstring_view FilterPattern;

    const FileType fileType;
    const FolderType folderType;
    const wstring key;
    const wstring text;
    const wstring filterText;
    const wstring filterExtensions; // In the form ".abc;.xyz"
    const wstring defaultExtension; // In the form ".abc"

    FilterPattern GetFilterPattern();

private:
    wstring filterPattern;

    friend class  FileTypeFactory;

    FileTypeInfo(FileType fileType,
        wstring_view key,
        wstring_view text,
        wstring_view filterText,
        wstring_view filterExtensions,
        wstring_view defaultExtension,
        FolderType folderType
    );
};

class FileTypeFactory {
public:
    static FileTypeInfo GetInfo(wstring_view key);
    static FileTypeInfo GetInfo(const FileType type);
};
