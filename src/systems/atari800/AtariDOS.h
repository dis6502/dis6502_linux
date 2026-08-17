/*
 * AtariDOS.h
 * Atari DOS include file.
 */
#pragma once


#include "../../DatatypeUtility.h"
#include <cstdint>
#include "../../FileIO.h"
#include "../../Memory.h"
#include "../../String.h"
#include "../../Syntax.h"

#include <gsl/pointers>

 /*
  * error codes
  */
enum class AtariError {
    OK = 0,
    NO_ENTRY_FOUND,
    END_OF_FILE,
    DISK_NOT_FOUND,
    DIRECTORY_NOT_FOUND,
    DIRECTORY_READ,
    DIRECTORY_WRITE,
    INVALID_VTOC_ENTRY,
    SECTOR_NOT_FOUND,
    FILE_READ,
    FILE_WRITE,
    FILE_CORRUPTED,
    FILE_ALREADY_EXISTS,
    NO_FREE_SECTOR,
    BITMAP_READ,
    BITMAP_WRITE,
    SECTOR_ALREADY_FREE,
    FILE_SEEK
};

class AtariFile;
class AtariDisk;

class AtariDOS {
public:

    using sector_number = uint16_t;    // two bytes
    using sector_count = uint16_t;     // two bytes

    using sector_size = uint16_t;      // two bytes
    using sector_offset = uint16_t;    // two bytes

    using directory_index = int;       // -1 means invvalid, value values are 0..63

    using file_size = uint32_t;

    using file_attribute = byte;
    using file_attribute_bit = byte;

    // File attributes.
    typedef enum FileAttribute : file_attribute_bit {
        OPEN_FOR_OUTPUT = 0x01,
        UNKNOWN = 0x02,
        LOCKED = 0x20,
        IN_USE = 0x40,
        DELETED = 0x80
    } FileAttribute;

    static gsl::not_null<const wchar_t*> GetErrorCode(AtariError atariError);
    static wstring GetErrorText(AtariError atariError);

    static wstring GetFileName83(wstring_view fileName);

    static std::unique_ptr<AtariDisk> OpenAtariDisk(wstring_view diskImageFilePath);

private:
    friend AtariDisk;

    static constexpr uint8_t directory_entries_per_sector = 8;

    // Methods used by AtariDisk
    static AtariError FindFirst(wstring_view diskImageFilePath, AtariFile& info);
    static AtariError FindNext(wstring_view diskImageFilePath, AtariFile& info);

    static AtariError GetFileFromIndex(wstring_view diskImageFilePath, AtariFile& info, directory_index directoryIndex);
    static AtariError ReadFirstSector(wstring_view diskImageFilePath, AtariFile& info);
    static AtariError ReadNextSector(wstring_view diskImageFilePath, AtariFile& info);
    static AtariError CheckFile(wstring_view diskImageFilePath, AtariFile& info, wstring_view fileName);
    static AtariError CreateFile(wstring_view diskImageFilePath, AtariFile& info, wstring_view fileName);
    static AtariError WriteSector(wstring_view diskImageFilePath, AtariFile& info);
    static AtariError GetFileSize(wstring_view diskImageFilePath, AtariFile& info, file_size& fileSize);
    static AtariError ReadFile(wstring_view diskImageFilePath, AtariFile& info, int& iDiskIndex, byte* fileBuffer, file_size fileSize);

    // Private methods use file descriptors.
    static AtariError Seek(FILE* fd, sector_number sectorNumber); // BEWARE!! sectorNumber is starting from 0 instead of 1
    static AtariError FindDirEntry(FILE* fd, AtariFile& info);
    static AtariError GetFreeDirEntry(FILE* fd, directory_index& directoryIndex);
    static AtariError WriteDirEntry(FILE* fd, AtariFile& info);
    static byte       AllocBit(byte& bitmask);
    static AtariError AllocSector(FILE* fd, sector_number& sectorNumber, directory_index directoryIndex);
    static AtariError FreeSector(FILE* fd, sector_number sectorNumber);
    static AtariError SeekEndOfFile(FILE* fd, AtariFile& info);
    static AtariError ReadSector(FILE* fd, AtariFile& info);
};

/*
 * Contain class for AtariFindFirst and AtariFindNext.
 */
class AtariDOS;

class AtariFile {

public:

    AtariFile();
    void Clear();

    AtariDOS::directory_index GetDirectoryIndex() const;

    // File attrobutes
    bool IsOpenForInput();
    bool IsUnknown();
    bool IsLocked();
    bool IsInUse();
    bool IsDeleted();

    AtariDOS::sector_count GetSectorCount() const;
    AtariDOS::sector_number GetStartSectorNumber() const;
    //AtariDOS::file_size GetFileSize() const;

    wstring GetFileName(); // Formatted
    wstring GetFileName83(); // Unformatted 8.3 characters
    wstring GetDirectoryText(); // Formatted as "* FILENAME.EXT 123"

private:
    friend AtariDOS;

    //  Entry index in the directory (0-63) or -1 if undefined
    AtariDOS::directory_index directoryIndex;

    AtariDOS::file_attribute fileAttribute;
    AtariDOS::sector_count sectorCount;
    AtariDOS::sector_number startSectorNumber;
    wstring fileName;

    bool IsAttributeSet(AtariDOS::file_attribute_bit fileAttributeBit) const;

    // Current director sector data
    byte szSector[128];
    // Number of bytes used in the sector buffer
    Memory::word sectorSize;
    // Next sector where the file continues, or 0 if this is the last sector
    AtariDOS::sector_number nextSectorNumber;
};


class AtariDisk {

public:

    AtariDisk(wstring_view diskImageFilePath);
    wstring GetDiskImageFilePath() const;
    ByteArray ReadFile(wstring_view fileName); // Read the first, non-deleted file with the given file name; throws IOException

    // The methods below are primarily for internal purposes to iterate over the directoy of the disk
    AtariError FindFirst(AtariFile& info);
    AtariError FindNext(AtariFile& info);
    AtariError GetFileFromIndex(AtariFile& info, AtariDOS::directory_index directoryIndex);
    AtariError ReadFirstSector(AtariFile& info);
    AtariError ReadNextSector(AtariFile& info);
    AtariError CheckFile(AtariFile& info, wstring_view fileName);
    AtariError CreateFile(AtariFile& info, wstring_view fileName);
    AtariError WriteSector(AtariFile& info);
    AtariError GetFileSize(AtariFile& info, AtariDOS::file_size& fileSize);
    AtariError ReadFile(AtariFile& info, int& iDiskIndex, byte* lpBuffer, AtariDOS::file_size fileSize);


private:
    friend AtariDOS;

    wstring diskImageFilePath;

};