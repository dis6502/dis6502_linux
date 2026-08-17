#include <map>
#include <sstream>  

#include "FileType.h"
#include "FolderType.h"

FileTypeInfo::FileTypeInfo(FileType fileType,
    wstring_view key,
    wstring_view text,
    wstring_view filterText,
    wstring_view filterExtensions,
    wstring_view defaultExtension,
    FolderType folderType
) :
    fileType{ fileType },
    folderType{ folderType },
    key{ key },
    text{ text },
    filterText{ filterText },
    filterExtensions{ filterExtensions },
    defaultExtension{ defaultExtension }
{ }

FileTypeInfo::FilterPattern FileTypeInfo::GetFilterPattern() {
    if (filterPattern.empty()) {

        /*
           A buffer containing pairs of null - terminated filter strings.
           The last string in the buffer must be terminated by two NULL characters.

           The first string in each pair is a display string that describes the filter(for example, "Text Files"),
           and the second string specifies the filter pattern(for example, ".TXT").
           To specify multiple filter patterns for a single display string,
           use a semicolon to separate the patterns(for example, ".TXT;.DOC;.BAK").
           A pattern string can be a combination of valid file name characters and the asterisk(*) wildcard character.
           Do not include spaces in the pattern string.

           The system does not change the order of the filters.It displays them in the File Types combo box in the order specified in lpstrFilter.
        */
        auto pattern = filterText;
        wstringstream ss(filterExtensions);
        wstring extensions;
        while (ss.good()) {
            if (!extensions.empty()) {
                extensions += L";";
            }
            wstring extension;
            getline(ss, extension, L';');
            if (!extension.empty()) {
                extensions += L"*" + extension;
            }
        }
        if (!pattern.empty()) {
            pattern += L" (" + extensions + L")";
            pattern += L'\0';
            pattern += extensions;
            pattern += L'\0';
        }
        pattern += L"All files";
        pattern += L'\0';
        pattern += L"*.*";
        pattern += L'\0';
        pattern += L'\0';
        this->filterPattern = pattern;
    }
    return wstring_view(filterPattern.data(), filterPattern.size());
}

FileTypeInfo FileTypeFactory::GetInfo(wstring_view key) {
    static std::map<wstring_view, FileType> typeLookup = {
        { L"RAW_FILE", FileType::RAW_FILE },
        { L"EXECUTABLE_FILE", FileType::EXECUTABLE_FILE },
        { L"ROM_IMAGE_FILE", FileType::ROM_IMAGE_FILE },
        { L"CASSETTE_IMAGE_FILE", FileType::CASSETTE_IMAGE_FILE },
        { L"DISK_IMAGE_EXECUTABLE_FILE", FileType::DISK_IMAGE_EXECUTABLE_FILE },
        { L"DISK_IMAGE_BOOT_SECTORS", FileType::DISK_IMAGE_BOOT_SECTORS },
        { L"DISK_IMAGE_SECTORS", FileType::DISK_IMAGE_SECTORS },
        { L"WORKSPACE_FILE", FileType::WORKSPACE_FILE },
        { L"EQUATES_FILE", FileType::EQUATES_FILE },
        { L"PROFILE_FILE", FileType::PROFILE_FILE },
        { L"DISASSEMBLY_FILE", FileType::DISASSEMBLY_FILE }
    };

    FileType fileType = FileType::UNKNOWN_FILE;
    try {
        fileType = typeLookup.at(key);
    }
    catch (const std::out_of_range&) {}

    return GetInfo(fileType);
}

FileTypeInfo FileTypeFactory::GetInfo(const FileType type) {
    if (type == FileType::RAW_FILE)
        return FileTypeInfo(FileType::RAW_FILE, L"RAW_FILE", L"Raw File", L"Raw Files", L".*", L".bin", FolderType::RAW_FILES);
    else if (type == FileType::EXECUTABLE_FILE)
        return FileTypeInfo(FileType::EXECUTABLE_FILE, L"EXECUTABLE_FILE", L"Executable File", L"Executable Files", L".bin;.com;.exe;.sys;.xex", L".xex", FolderType::EXECUTABLE_FILES);
    else if (type == FileType::ROM_IMAGE_FILE)
        return FileTypeInfo(FileType::ROM_IMAGE_FILE, L"ROM_IMAGE_FILE", L"ROM Image File", L"ROM Image Files", L".bin;.car;.rom", L".rom", FolderType::ROM_IMAGE_FILES);
    else if (type == FileType::CASSETTE_IMAGE_FILE)
        return FileTypeInfo(FileType::CASSETTE_IMAGE_FILE, L"CASSETTE_IMAGE_FILE", L"Cassette Image File", L"Cassette Image Files", L".cas", L".cas", FolderType::CASSETTE_IMAGE_FILES);
    else if (type == FileType::DISK_IMAGE_EXECUTABLE_FILE)
        return FileTypeInfo(FileType::DISK_IMAGE_EXECUTABLE_FILE, L"DISK_IMAGE_EXECUTABLE_FILE", L"Disk Image Executable File", L"Disk Image Files", L".atr;.xfd", L".atr", FolderType::DISK_IMAGE_FILES);
    else if (type == FileType::DISK_IMAGE_BOOT_SECTORS)
        return FileTypeInfo(FileType::DISK_IMAGE_BOOT_SECTORS, L"DISK_IMAGE_BOOT_SECTORS", L"Disk Image Boot Sectors", L"Disk Image Files", L".atr;.xfd", L".atr", FolderType::DISK_IMAGE_FILES);
    else if (type == FileType::DISK_IMAGE_SECTORS)
        return FileTypeInfo(FileType::DISK_IMAGE_SECTORS, L"DISK_IMAGE_SECTORS", L"Disk Image Sectors", L"Disk Image Files", L".atr;.xfd", L".atr", FolderType::DISK_IMAGE_FILES);
    else if (type == FileType::WORKSPACE_FILE)
        return FileTypeInfo(FileType::WORKSPACE_FILE, L"WORKSPACE_FILE", L"Workspace File", L"Workspace Files", L".wrk", L".wrk", FolderType::WORKSPACE_FILES);
    else if (type == FileType::EQUATES_FILE)
        return FileTypeInfo(FileType::EQUATES_FILE, L"EQUATES_FILE", L"Equates File", L"Equates Files", L".equ", L".equ", FolderType::EQUATES_FILES);
    else if (type == FileType::PROFILE_FILE)
        return FileTypeInfo(FileType::PROFILE_FILE, L"PROFILE_FILE", L"Profile File", L"Profile Files", L".prf", L".prf", FolderType::PROFILE_FILES);
    else if (type == FileType::DISASSEMBLY_FILE)
        return FileTypeInfo(FileType::DISASSEMBLY_FILE, L"DISASSEMBLY_FILE", L"Disassembly File", L"Disassembly Files", L".asm", L".asm", FolderType::DISASSEMBLY_FILES);

    return FileTypeInfo(FileType::UNKNOWN_FILE, L"UNKNOWN_FILE", L"Unknown", L"All", L".*", L".*", FolderType::UNKNOWN_FILES);
}
