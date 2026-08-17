#include "AtariDOS.h"
#include "../../PlatformCompat.h"
#include <iomanip>
#include <sstream>

/* See also https ://github.com/jhallen/atari-tools for infos */

AtariFile::AtariFile() {
    Clear();
}

void AtariFile::Clear() {
    directoryIndex = -1;
    fileAttribute = 0;
    sectorCount = 0;
    startSectorNumber = 0;
    fileName.clear();

    DatatypeUtility::ClearByteArray(szSector, sizeof(szSector));
    sectorSize = 0;
    nextSectorNumber = 0;
}

gsl::not_null<const wchar_t*> AtariDOS::GetErrorCode(AtariError atariError) {
    switch (atariError) {
    case AtariError::OK:
        return L"OK";

    case AtariError::NO_ENTRY_FOUND:
        return L"NO_ENTRY_FOUND";

    case AtariError::END_OF_FILE:
        return L"END_OF_FILE";

    case AtariError::DISK_NOT_FOUND:
        return L"DISK_NOT_FOUND";

    case AtariError::DIRECTORY_NOT_FOUND:
        return L"DIRECTORY_NOT_FOUND";

    case AtariError::DIRECTORY_READ:
        return L"DIRECTORY_READ";

    case AtariError::DIRECTORY_WRITE:
        return L"DIRECTORY_WRITE";

    case AtariError::INVALID_VTOC_ENTRY:
        return L"INVALID_VTOC_ENTRY";

    case AtariError::SECTOR_NOT_FOUND:
        return L"SECTOR_NOT_FOUND";

    case AtariError::FILE_READ:
        return L"FILE_READ";

    case AtariError::FILE_WRITE:
        return L"FILE_WRITE";

    case AtariError::FILE_CORRUPTED:
        return L"FILE_CORRUPTED";

    case AtariError::FILE_ALREADY_EXISTS:
        return L"FILE_ALREADY_EXISTS";

    case AtariError::NO_FREE_SECTOR:
        return L"NO_FREE_SECTOR";

    case AtariError::BITMAP_READ:
        return L"BITMAP_READ";

    case AtariError::BITMAP_WRITE:
        return L"BITMAP_WRITE";

    case AtariError::SECTOR_ALREADY_FREE:
        return L"SECTOR_ALREADY_FREE";

    case AtariError::FILE_SEEK:
        return L"FILE_SEEK";

    default:
        return L"UNKNOWN";
    }
}

wstring AtariDOS::GetErrorText(AtariError atariError) {
    switch (atariError) {
    case AtariError::OK:
        return L"OK";

    case AtariError::NO_ENTRY_FOUND:
        return L"No matching directory entry found";

    case AtariError::END_OF_FILE:
        return L"End of file reached)";

    case AtariError::DISK_NOT_FOUND:
        return L"Disk not found";

    case AtariError::DIRECTORY_NOT_FOUND:
        return L"Directory not found";

    case AtariError::DIRECTORY_READ:
        return L"Cannot read directory";

    case AtariError::DIRECTORY_WRITE:
        return L"Cannot write directory";

    case AtariError::INVALID_VTOC_ENTRY:
        return L"Invalid VTOC entry";

    case AtariError::SECTOR_NOT_FOUND:
        return L"Sector not found";

    case AtariError::FILE_READ:
        return L"Cannot read file";

    case AtariError::FILE_WRITE:
        return L"Cannot write file";

    case AtariError::FILE_CORRUPTED:
        return L"File structure is corrupted";

    case AtariError::FILE_ALREADY_EXISTS:
        return L"File already exists";

    case AtariError::NO_FREE_SECTOR:
        return L"No free sector";

    case AtariError::BITMAP_READ:
        return L"Cannot read bitmap";

    case AtariError::BITMAP_WRITE:
        return L"Cannot write bitmap";

    case AtariError::SECTOR_ALREADY_FREE:
        return L"Sector is already free";

    case AtariError::FILE_SEEK:
        return L"Cannot seek file position";

    default:
        return L"Unkown error";
    }
}

wstring AtariDOS::GetFileName83(wstring_view fileName) {

    if (fileName.empty()) {
        return String::Empty();
    }

    wstring result = L"        .   ";
    size_t i = 0;
    size_t offset = 0;
    while (i < fileName.length() && fileName.at(i) != '.') {
        const wchar_t c = fileName.at(i++);
        if (c != '.') {
            if (offset < 8) {
                result.at(offset++) = c;
            }
        }
        else {
            break;
        }
    }
    while ((i < fileName.length() && fileName.at(i++) != '.')) {
    }
    offset = 9;
    while (i < fileName.length() && offset < 12) {
        result.at(offset++) = fileName.at(i++);
    }

    return result;
}

AtariDOS::directory_index AtariFile::GetDirectoryIndex() const {
    return directoryIndex;
}

bool AtariFile::IsOpenForInput() {
    return IsAttributeSet(AtariDOS::FileAttribute::OPEN_FOR_OUTPUT);
}

bool AtariFile::IsUnknown() {
    return IsAttributeSet(AtariDOS::FileAttribute::UNKNOWN);
}

bool AtariFile::IsLocked() {
    return IsAttributeSet(AtariDOS::FileAttribute::LOCKED);
}
bool AtariFile::IsInUse() {
    return IsAttributeSet(AtariDOS::FileAttribute::IN_USE);
}

bool AtariFile::IsDeleted() {
    return IsAttributeSet(AtariDOS::FileAttribute::DELETED);
}

bool AtariFile::IsAttributeSet(AtariDOS::file_attribute_bit fileAttributeBit) const {
    return (fileAttribute & fileAttributeBit) == fileAttributeBit;

}

AtariDOS::sector_count AtariFile::GetSectorCount() const {
    return sectorCount;
}

AtariDOS::sector_number AtariFile::GetStartSectorNumber() const {
    return startSectorNumber;
}

wstring AtariFile::GetFileName() {
    return fileName;
}

wstring AtariFile::GetFileName83() {
    return AtariDOS::GetFileName83(fileName);
}

wstring AtariFile::GetDirectoryText() {
    wstringstream result;
    result << (IsLocked() ? "*" : " ") << " " << GetFileName83() << " " << std::setfill(L'0') << std::setw(3) << GetSectorCount();
    return result.str();
}


AtariDisk::AtariDisk(wstring_view diskImageFilePath) {
    this->diskImageFilePath = diskImageFilePath;
}
wstring AtariDisk::GetDiskImageFilePath() const {
    return diskImageFilePath;
}

AtariError AtariDisk::FindFirst(AtariFile& info) {
    return AtariDOS::FindFirst(diskImageFilePath, info);
}
AtariError AtariDisk::FindNext(AtariFile& info) {
    return AtariDOS::FindNext(diskImageFilePath, info);
}

AtariError AtariDisk::GetFileFromIndex(AtariFile& info, AtariDOS::directory_index directoryIndex) {
    return AtariDOS::GetFileFromIndex(diskImageFilePath, info, directoryIndex);
}

AtariError AtariDisk::ReadFirstSector(AtariFile& info) {
    return AtariDOS::ReadFirstSector(diskImageFilePath, info);
}

AtariError AtariDisk::ReadNextSector(AtariFile& info) {
    return AtariDOS::ReadNextSector(diskImageFilePath, info);
}

AtariError AtariDisk::CheckFile(AtariFile& info, wstring_view fileName) {
    return AtariDOS::CheckFile(diskImageFilePath, info, fileName);
}

AtariError AtariDisk::CreateFile(AtariFile& info, wstring_view fileName) {
    return AtariDOS::CreateFile(diskImageFilePath, info, fileName);
}

AtariError AtariDisk::WriteSector(AtariFile& info) {
    return AtariDOS::WriteSector(diskImageFilePath, info);

}
AtariError AtariDisk::GetFileSize(AtariFile& info, AtariDOS::file_size& fileSize) {
    return AtariDOS::GetFileSize(diskImageFilePath, info, fileSize);
}

// Use iDiskIndex = 1 to read from the start
AtariError AtariDisk::ReadFile(AtariFile& info, int& iDiskIndex, byte* buffer, AtariDOS::file_size fileSize) {

    return AtariDOS::ReadFile(diskImageFilePath, info, iDiskIndex, buffer, fileSize);
}

ByteArray AtariDisk::ReadFile(wstring_view fileName) {
    AtariFile atariFile;

    auto error = CheckFile(atariFile, fileName);
    if (error != AtariError::OK) {
        throw IOException(AtariDOS::GetErrorText(error));
    }
    AtariDOS::file_size atariFileSize = 0;
    error = GetFileSize(atariFile, atariFileSize);
    if (error != AtariError::OK) {
        throw IOException(AtariDOS::GetErrorText(error));
    }

    auto result = ByteArray(atariFileSize);
    if (atariFileSize > 0) {
        int iDiskIndex = -1;
        error = ReadFile(atariFile, iDiskIndex, result.get(), result.size());
        if (error != AtariError::OK) {
            throw IOException(AtariDOS::GetErrorText(error));
        }
    }
    return result;
}

std::unique_ptr<AtariDisk>AtariDOS::OpenAtariDisk(wstring_view diskImageFilePath) {
    return std::make_unique<AtariDisk>(diskImageFilePath);
}

AtariError AtariDOS::Seek(FILE* fd, sector_number sectorNumber) {
    struct _stat buf;

    // determine if we have an XFD or ATR file
    if (_fstat(_fileno(fd), &buf)) {
        return AtariError::SECTOR_NOT_FOUND;
    }

    // go to sector
    if (buf.st_size == 92160L || buf.st_size == 133120L) {
        if (fseek(fd, sectorNumber * 128L, SEEK_SET)) {
            return AtariError::SECTOR_NOT_FOUND;
        }
    }
    else if (buf.st_size == 92176L || buf.st_size == 133136L) {
        if (fseek(fd, (sectorNumber * 128L) + 16L, SEEK_SET)) {
            return AtariError::SECTOR_NOT_FOUND;
        }
    }
    else {
        return AtariError::SECTOR_NOT_FOUND;
    }

    return AtariError::OK;
}

AtariError AtariDOS::FindDirEntry(FILE* fd, AtariFile& info) {
    unsigned wEntry, wFirstEntry;
    unsigned char sector[128];

    // go to the next entry in the directory.
    info.directoryIndex++;
    if (info.directoryIndex < 0 || info.directoryIndex > 63) {
        return AtariError::NO_ENTRY_FOUND;
    }

    // scan all directory sectors to find the first valid entry.
    const sector_number firstSectorNumber = 360 + (info.directoryIndex / 8);
    for (auto sectorNumber = firstSectorNumber; sectorNumber < 368; sectorNumber++) {
        // go to the directory sector.
        if (Seek(fd, sectorNumber) != AtariError::OK) {
            return AtariError::DIRECTORY_NOT_FOUND;
        }

        // the sector is read.
        if (fread(sector, 128, 1, fd) != 1) {
            return AtariError::DIRECTORY_READ;
        }

        // scan all entries of this directory sector (8 entries).
        if (sectorNumber == firstSectorNumber) {
            wFirstEntry = info.directoryIndex % 8;
        }
        else {
            wFirstEntry = 0;
        }

        for (wEntry = wFirstEntry; wEntry < 8; wEntry++) {
            /*
            * the file status is checked to determine if the file is
            * valid (not deleted).
            */
            size_t offset = wEntry * 16;
            if ((sector[offset]) && ((sector[offset] & FileAttribute::DELETED) == 0)) {
                info.Clear();
                offset += 5;
                for (int index = 0; index < 8; index++) {
                    const char c = sector[offset + index];
                    if (c != ' ') {
                        info.fileName.append(1, c);
                    }
                    else {
                        // SPACE defines file name end in DOS 2.x file names
                        break;
                    }
                }

                info.fileName.append(L".");
                offset += 8;
                for (int index = 0; index < 3; index++) {
                    const char c = sector[offset + index];
                    if (c != ' ') {
                        info.fileName.append(1, c);
                    }
                    else {
                        break;
                    }
                }

                /*
                * get the file size in number of sectors and keep the
                * index of the file entry so AtariFileNext knows where
                * to start.
                */
                info.directoryIndex = ((sectorNumber - 360) * 8) + wEntry;
                const sector_offset sectorOffset = wEntry * 16;
                info.fileAttribute = sector[sectorOffset];
                info.sectorCount = (sector[sectorOffset + 2] * 256) + sector[sectorOffset + 1];
                info.startSectorNumber = ((sector[sectorOffset + 4] * 256) + sector[sectorOffset + 3]);

                return AtariError::OK;
            }
        }
    }

    return AtariError::NO_ENTRY_FOUND;
}

AtariError AtariDOS::GetFreeDirEntry(FILE* fd, directory_index& directoryIndex) {
    unsigned char sector[128];

    directoryIndex = -1;

    // scan all directory sectors to find the first free entry.
    for (sector_number sectorNumber = 360; sectorNumber < 368; sectorNumber++) {
        // go to the directory sector.
        if (Seek(fd, sectorNumber) != AtariError::OK) {
            return AtariError::DIRECTORY_NOT_FOUND;
        }

        // the sector is read.
        if (fread(sector, 128, 1, fd) != 1) {
            return AtariError::DIRECTORY_READ;
        }

        // scan all entries of this directory sector (8 entries).
        for (directory_index entryNumber = 0; entryNumber < directory_entries_per_sector; entryNumber++)
            // the file status is checked to determine if the entry is free.
            if ((sector[(entryNumber * 16)] & FileAttribute::IN_USE) == 0) {
                // return the index found.
                directoryIndex = ((sectorNumber - 360) * directory_entries_per_sector) + entryNumber;

                return AtariError::OK;
            }
    }

    return AtariError::NO_ENTRY_FOUND;
}

AtariError AtariDOS::WriteDirEntry(FILE* fd, AtariFile& info) {
    unsigned wEntry;
    unsigned wLng, wExt;
    byte sector[128];

    // find directory sector.
    const sector_number sectorNumber = 360 + (info.directoryIndex / 8);

    // go to the directory sector.
    if (Seek(fd, sectorNumber) != AtariError::OK) {
        return AtariError::DIRECTORY_NOT_FOUND;
    }

    // the sector is read.
    if (fread(sector, 128, 1, fd) != 1) {
        return AtariError::DIRECTORY_READ;
    }

    // find directory entry in the sector.
    wEntry = (unsigned)info.directoryIndex % 8;

    // write entry.
    sector[(wEntry * 16)] = info.fileAttribute;
    sector[(wEntry * 16) + 1] = info.sectorCount & 0xFF;
    sector[(wEntry * 16) + 2] = (info.sectorCount >> 8) & 0xFF;
    sector[(wEntry * 16) + 3] = info.startSectorNumber & 0xFF;
    sector[(wEntry * 16) + 4] = (info.startSectorNumber >> 8) & 0xFF;

    memset(&(sector[(wEntry * 16) + 5]), ' ', 11 * sizeof(byte));

    for (wLng = 0; (wLng < 8) && (info.fileName[wLng]) && (info.fileName[wLng] != '.'); wLng++) {
        sector[(wEntry * 16) + 5 + wLng] = toupper(info.fileName[wLng]);
    }

    if (info.fileName[wLng] == '.') {
        wLng++;
    }

    for (wExt = 0; (wExt < 3) && (info.fileName[wLng]); wExt++, wLng++) {
        sector[(wEntry * 16) + 13 + wExt] = toupper(info.fileName[wLng]);
    }

    // go to the directory sector.
    if (Seek(fd, sectorNumber) != AtariError::OK) {
        return AtariError::DIRECTORY_NOT_FOUND;
    }

    // the sector is written.
    if (fwrite(sector, 128, 1, fd) != 1) {
        return AtariError::DIRECTORY_WRITE;
    }

    return AtariError::OK;
}

byte AtariDOS::AllocBit(byte& bitmask) {
    auto mask = byte{ 0x80 };

    for (byte index = 0; index < 8; index++) {
        if (bitmask & mask) {
            bitmask &= ~mask;
            return index;
        }

        mask >>= 1;
    }

    return 0;
}

AtariError AtariDOS::AllocSector(FILE* fd, sector_number& sectorNumber, directory_index directoryIndex) {
    byte sector[128];
    byte data[128];

    // go to sector.
    auto error = Seek(fd, 359L);
    if (error != AtariError::OK) {
        return error;
    }

    // the sector is read.
    if (fread(sector, 128, 1, fd) != 1) {
        return AtariError::FILE_READ;
    }

    // check if there is a free sector.
    if ((sector[3] == 0) && (sector[4] == 0)) {
        return AtariError::NO_FREE_SECTOR;
    }

    // find a free sector.
    for (Memory::offset wIndex = 10; wIndex <= 125; wIndex++) {
        if (sector[wIndex]) {
            // return the sector number
            sectorNumber = ((wIndex - 10) * 8) + AllocBit(sector[wIndex]);

            // write a blank sector with the directory entry number filled.
            memset(data, 0, sizeof(data));
            data[125] = ((byte)directoryIndex & 0x3F) << 2;

            // go to sector.
            error = Seek(fd, sectorNumber - 1);
            if (error != AtariError::OK) {
                return error;
            }

            // the sector is written.
            if (fwrite(data, 128, 1, fd) != 1) {
                return AtariError::BITMAP_READ;
            }

            // decrement the free sector count.
            if (sector[3]) {
                sector[3] = sector[3] - 1;
            }
            else {
                sector[3] = 0xFF;
                sector[4] = sector[4] - 1;
            }

            // go to sector.
            error = Seek(fd, 359L);
            if (error != AtariError::OK) {
                return error;
            }

            // the sector is written.
            if (fwrite(sector, 128, 1, fd) != 1) {
                return AtariError::BITMAP_WRITE;
            }

            return AtariError::OK;
        }
    }

    return AtariError::NO_FREE_SECTOR;
}

AtariError AtariDOS::FreeSector(FILE* fd, sector_number sectorNumber) {
    unsigned wIndex, wBit, wMask;
    byte sector[128];

    // go to sector.
    auto error = Seek(fd, 359L);
    if (error != AtariError::OK) {
        return error;
    }

    // the sector is read.
    if (fread(sector, 128, 1, fd) != 1) {
        return AtariError::BITMAP_READ;
    }

    // determine index into sector and bit number
    wIndex = ((sectorNumber - 1) / 8) + 10;
    wBit = (sectorNumber - 1) % 8;
    wMask = 0x80 >> wBit;

    // check if the sector is in use.
    if (sector[wIndex] & wMask) {
        return AtariError::SECTOR_ALREADY_FREE;
    }

    // turn bit on.
    sector[wIndex] |= wMask;

    // increment the free sector count.
    if (sector[3] != 0xFF) {
        sector[3] = sector[3] + 1;
    }
    else {
        sector[3] = 0;
        sector[4] = sector[4] + 1;
    }

    // go to sector.
    error = Seek(fd, 359L);
    if (error != AtariError::OK) {
        return error;
    }

    // the sector is read.
    if (fwrite(sector, 128, 1, fd) != 1) {
        return AtariError::BITMAP_WRITE;
    }

    return AtariError::OK;
}

AtariError AtariDOS::SeekEndOfFile(FILE* fd, AtariFile& info) {
    byte sector[128];

    // go to sector.
    auto error = Seek(fd, info.nextSectorNumber - 1);
    if (error != AtariError::OK) {
        return error;
    }

    // the sector is read.
    if (fread(sector, 128, 1, fd) != 1) {
        return AtariError::FILE_SEEK;
    }

    /*
    * if the sector is empty, we write this one.
    * Otherwise, we must allocate a new one.
    */
    if (sector[127] & 0x7F) {
        sector_number sectorNumber;
        constexpr directory_index directoryIndex = 0;
        error = AllocSector(fd, sectorNumber, directoryIndex);
        if (error != AtariError::OK) {
            return error;
        }

        // the last sector must be linked with the new one.
        sector[125] &= ~0x03;
        sector[125] |= (sectorNumber >> 8) & 0x03;
        sector[126] = sectorNumber & 0xFF;
        error = Seek(fd, info.nextSectorNumber - 1);
        if (error != AtariError::OK) {
            return error;
        }

        // the sector is written.
        if (fwrite(sector, 128, 1, fd) != 1) {
            return AtariError::FILE_WRITE;
        }

        // tell the caller which is the last sector.
        info.nextSectorNumber = sectorNumber;

        // increment the sector count in the directory.
        info.sectorCount++;
        error = WriteDirEntry(fd, info);
        if (error != AtariError::OK) {
            return error;
        }
    }

    return AtariError::OK;
}

AtariError AtariDOS::ReadSector(FILE* fd, AtariFile& info) {
    long directoryIndex;

    // check for end of file.
    info.sectorSize = 0;
    if (info.nextSectorNumber == 0) {
        return AtariError::END_OF_FILE;
    }

    // go to sector.
    const auto error = Seek(fd, info.nextSectorNumber - 1);
    if (error != AtariError::OK) {
        return error;
    }

    // the sector is read.
    if (fread(info.szSector, 128, 1, fd) != 1) {
        return AtariError::FILE_READ;
    }

    // check if the sector belongs to the file
    directoryIndex = ((info.szSector[125] >> 2) & 0x3F);
    if (directoryIndex != info.directoryIndex) {
        return AtariError::FILE_CORRUPTED;
    }

    /*
    * determination de la taille des données, du secteur suivant et
    * ecriture du secteur dans le fichier PC destination.
    */
    info.sectorSize = (info.szSector[127] & 0x7F);

    if ((info.szSector[127]) & 0x80) {
        info.nextSectorNumber = 0;
    }
    else {
        info.nextSectorNumber = ((info.szSector[125] & 0x03) << 8) + info.szSector[126];
    }

    return AtariError::OK;
}


FILE* OpenForRead(wstring_view diskImageFilePath) {
    return FileIO::OpenFile(diskImageFilePath, L"rb");
}
FILE* OpenForReadWrite(wstring_view diskImageFilePath) {
    return FileIO::OpenFile(diskImageFilePath, L"r+b");
}

/*
* fill the structure with the first entry of the directory.
* Files that have been deleted or open for output are skipped.
*/
AtariError AtariDOS::FindFirst(wstring_view diskImageFilePath, AtariFile& info) {
    // Open Atari disk file (*.atr, *.xfd).
    FILE* fd;
    try {
        fd = OpenForRead(diskImageFilePath);
    }
    catch (const IOException&) {
        return AtariError::DISK_NOT_FOUND;
    }

    // search from the first entry.
    info.directoryIndex = -1;

    // fill info structure.
    const auto error = FindDirEntry(fd, info);

    FileIO::CloseFile(fd);

    return error;
}

/*
* fill the structure with the next entry of the directory.
* Files that have been deleted or open for output are skipped.
*/
AtariError AtariDOS::FindNext(wstring_view diskImageFilePath, AtariFile& info) {
    // open Atari disk file (*.xfd).
    FILE* fd;
    if (fd = OpenForRead(diskImageFilePath); !fd) {
        return AtariError::DISK_NOT_FOUND;
    }

    // fill info structure.
    const auto error = FindDirEntry(fd, info);

    FileIO::CloseFile(fd);

    return error;
}

/*
* fill the structure with the entry at the given index.
* Files that have been deleted or open for output are skipped.
* If there is no file, INVALID_VTOC_ENTRY is returned.
*/
AtariError AtariDOS::GetFileFromIndex(wstring_view diskImageFilePath, AtariFile& info, directory_index directoryIndex) {
    // open Atari disk file (*.xfd).
    FILE* fd;
    if (fd = OpenForRead(diskImageFilePath); !fd) {
        return AtariError::DISK_NOT_FOUND;
    }

    // search from the first entry.
    info.directoryIndex = directoryIndex - 1;

    // fill info structure.
    auto error = FindDirEntry(fd, info);

    FileIO::CloseFile(fd);

    // verify that we have found the entry expected by the user.
    if ((error == AtariError::OK) && (info.directoryIndex != directoryIndex)) {
        error = AtariError::INVALID_VTOC_ENTRY;
    }

    return error;
}

/*
* read first sector of a file (125 bytes).
*/
AtariError AtariDOS::ReadFirstSector(wstring_view diskImageFilePath, AtariFile& info) {
    // open Atari disk file (*.xfd).
    FILE* fd;
    if (fd = OpenForRead(diskImageFilePath); !fd) {
        return AtariError::DISK_NOT_FOUND;
    }

    // reset Atari file pointer to begining of file
    info.nextSectorNumber = info.startSectorNumber;

    // fill user buffer.
    const auto error = ReadSector(fd, info);

    FileIO::CloseFile(fd);

    return error;
}

/*
* read next sector of a file (125 bytes).
*/
AtariError AtariDOS::ReadNextSector(wstring_view diskImageFilePath, AtariFile& info) {
    // open Atari disk file (*.xfd).
    FILE* fd;
    if (fd = OpenForRead(diskImageFilePath); !fd) {
        return AtariError::DISK_NOT_FOUND;
    }

    // fill user buffer.
    const auto error = AtariDOS::ReadSector(fd, info);

    FileIO::CloseFile(fd);

    return error;
}

/*
* checks that the file does not exist.
*/
AtariError AtariDOS::CheckFile(wstring_view diskImageFilePath, AtariFile& info, wstring_view fileName) {
    // get first file name of the disk.
    AtariError error = AtariDOS::FindFirst(diskImageFilePath, info);

    while (error == AtariError::OK) {
        /*
        * check if the new file has the same name of one of
        * the files of the disk.
        */
        if (String::EqualsIgnoreCase(fileName, info.fileName)) {
            return AtariError::OK;
        }

        // get the next file name.
        error = AtariDOS::FindNext(diskImageFilePath, info);
    }

    return error;
}

/*
* Creates a new empty file.
*/
AtariError AtariDOS::CreateFile(wstring_view diskImageFilePath, AtariFile& info, wstring_view fileName) {
    AtariError error = CheckFile(diskImageFilePath, info, fileName);

    // Check if file already exist.
    switch (error) {
    case AtariError::OK:
        return AtariError::FILE_ALREADY_EXISTS;

    case AtariError::NO_ENTRY_FOUND:
        break;

    default:
        return error;
    }

    // Open Atari disk file.
    FILE* fd;
    if (fd = OpenForReadWrite(diskImageFilePath); !fd) {
        return AtariError::DISK_NOT_FOUND;
    }

    // Find a free directory entry.
    error = GetFreeDirEntry(fd, info.directoryIndex);
    if (error != AtariError::OK) {
        FileIO::CloseFile(fd);
        return error;
    }

    // Find a free sector for the first data sector of the file.
    error = AllocSector(fd, info.startSectorNumber, info.directoryIndex);
    if (error != AtariError::OK) {
        FileIO::CloseFile(fd);
        return error;
    }

    // fill this entry
    info.fileName = fileName;
    info.sectorCount = 1;
    info.fileAttribute = FileAttribute::IN_USE | FileAttribute::UNKNOWN;
    info.nextSectorNumber = info.startSectorNumber;

    // write entry into directory
    if (error = WriteDirEntry(fd, info); error != AtariError::OK) {
        FreeSector(fd, info.startSectorNumber);
    }

    FileIO::CloseFile(fd);

    return error;
}

AtariError AtariDOS::WriteSector(wstring_view diskImageFilePath, AtariFile& info) {
    FILE* fd;
    try {
        fd = OpenForReadWrite(diskImageFilePath);
    } catch (const IOException&) {
        return AtariError::DISK_NOT_FOUND;
    }

    // find last sector where we can write the data.
    auto error = SeekEndOfFile(fd, info);
    if (error != AtariError::OK) {
        FileIO::CloseFile(fd);
        return error;
    }

    // fill sector information
    info.szSector[125] = (info.directoryIndex & 0x3F) << 2;
    info.szSector[126] = 0;
    info.szSector[127] = info.sectorSize & 0x7F;

    // go to sector.
    error = Seek(fd, info.nextSectorNumber - 1);
    if (error != AtariError::OK) {
        FileIO::CloseFile(fd);
        return error;
    }

    // the sector is written.
    if (fwrite(info.szSector, 128, 1, fd) != 1) {
        error = AtariError::FILE_WRITE;
    }

    FileIO::CloseFile(fd);

    return error;
}

AtariError AtariDOS::GetFileSize(wstring_view diskImageFilePath, AtariFile& info, file_size& fileSize) {
    // Read all sectors of the file
    fileSize = 0;
    auto error = AtariDOS::ReadFirstSector(diskImageFilePath, info);

    while ((error == AtariError::OK) && (info.sectorSize > 0)) {
        fileSize += info.sectorSize;
        error = AtariDOS::ReadNextSector(diskImageFilePath, info);
    }

    if (error == AtariError::END_OF_FILE) {
        error = AtariError::OK;
    }

    if (error != AtariError::OK) {
        fileSize = 0;
    }

    return error;
}

AtariError AtariDOS::ReadFile(wstring_view diskImageFilePath, AtariFile& info, int& iDiskIndex, byte* fileBuffer, file_size fileSize) {
    auto error = AtariError::OK;

    // Read first sector if we are at the begining.
    if (iDiskIndex == -1) {
        error = ReadFirstSector(diskImageFilePath, info);

        if (error != AtariError::OK) {
            return error;
        }
        else {
            iDiskIndex = 0;
        }
    }

    // Copy bytes from sector to user buffer up to the specfied length.
    sector_offset wSectorLng = info.sectorSize - iDiskIndex;

    while (wSectorLng < fileSize) {
        if (wSectorLng) {
            memcpy(fileBuffer, &(info.szSector[iDiskIndex]), wSectorLng);
            fileBuffer += wSectorLng;
            fileSize -= wSectorLng;
        }

        iDiskIndex = 0;
        error = AtariDOS::ReadNextSector(diskImageFilePath, info);

        if (error != AtariError::OK) {
            info.sectorSize = 0;
            return error;
        }

        wSectorLng = info.sectorSize - iDiskIndex;
    }

    memcpy(fileBuffer, &(info.szSector[iDiskIndex]), fileSize);

    iDiskIndex += fileSize;

    return AtariError::OK;
}
