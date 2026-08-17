/*
 * Disk I/O operations based on ATRDLL.DLL
 */

#include "Application.h"
#include "DiskImage.h"
#include "FileIO.h"
#include "Memory.h"
#include <errno.h>
#include <iostream>

extern std::unique_ptr<Application> g_Application;


ImgError DiskImage::Seek(FILE* fd, const ImgInfo& info, const ImgRWPacket& sector) {
    Memory::dword dwOffset = 0;
    Memory::word wDensity = 0;

    // check sector number
    if ((sector.wSectorNumber < 1) || (sector.wSectorNumber > info.wSectors)) {
        return ImgError::OUT_OF_RANGE;
    }

    /*
    ** if user wants to read one of the three first sector of a double
    ** density disk, the size should be only 128 bytes.
    */
    if ((info.wDensity == 256) && (sector.wSectorNumber <= 3)) {
        wDensity = 128;
    }
    else {
        wDensity = info.wDensity;
    }

    // determine offset in the file
    switch (info.nResult) {
    case ImgError::XFD:
        dwOffset = (sector.wSectorNumber - 1) * wDensity;
        break;

    case ImgError::ATR:
        dwOffset = sizeof(ATRHeader) + ((sector.wSectorNumber - 1) * wDensity);
        break;
    default:
        return ImgError::DISK_ERROR;
    }

    // go to sector
    if (fseek(fd, (long)dwOffset, SEEK_SET)) {
        return ImgError::DISK_ERROR;
    }

    // everything is OK.
    return info.nResult;
}

ImgError DiskImage::Read(const ImgInfo& info, ImgRWPacket& sector) {
    ImgError iRet;

    // Open Atari disk file.
    FILE* fd;
    try {
        fd = FileIO::OpenFile(sector.filePath, L"rb");
    }
    catch (const IOException&) {
        return ImgError::FILE_NOT_FOUND;
    }

    // go to sector.
    if (IsError(iRet = Seek(fd, info, sector))) {
        FileIO::CloseFile(fd);
        return iRet;
    }

    // the sector is read.
    if (fread(sector.cSectorData.get(), info.wDensity, 1, fd) != 1) {
        iRet = ImgError::DISK_ERROR;
    }
    else {
        iRet = info.nResult;
    }

    FileIO::CloseFile(fd);

    return iRet;
}

ImgError DiskImage::Write(const ImgInfo& info, const ImgRWPacket& sector) {
    ImgError iRet;

    // Open Atari disk file.
    FILE* fd;
    try {
        fd = FileIO::OpenFile(sector.filePath, L"r+b");
    }
    catch (const IOException&) {
        // TOOD: Accessing this here is not really good.
        if (errno == EACCES) {
            return ImgError::WRITE_PROTECT;
        }
        else {
            return ImgError::FILE_NOT_FOUND;
        }
    }

    // Go to sector.
    if (IsError(iRet = Seek(fd, info, sector))) {
        FileIO::CloseFile(fd);

        return iRet;
    }

    // The sector is read.
    if (fwrite(sector.cSectorData.getConst(), info.wDensity, 1, fd) != 1) {
        iRet = ImgError::DISK_ERROR;
    }
    else {
        iRet = info.nResult;
    }

    FileIO::CloseFile(fd);

    return iRet;
}

/*
 * Get information about the disk image (sector size, disk type,...)
 */
void DiskImage::GetInfo(wstring_view filePath, ImgInfo& info) {

    if (!FileIO::FileExists(filePath)) {
        info.nResult = ImgError::FILE_NOT_FOUND;
        return;
    }

    // by default, the file is not recognized.
    info.nResult = ImgError::BAD_MAGIC;

    // check if the file is write-protected.
    info.cWriteProtect = FileIO::IsFileReadOnly(filePath);

    // check if this is an ATR file
    ByteArray fileContent;
    try {
        fileContent = FileIO::ReadByteArray(filePath);
    }
    catch (const IOException& ex) {
        info.nResult = ImgError::DISK_ERROR; // Todo have attribute for error text/exception
        return;
    }
    constexpr auto headerSize = sizeof(ATRHeader);
    if (fileContent.size() < headerSize) {
        info.nResult = ImgError::DISK_ERROR;
        return;
    }
    ATRHeader Atr;
    memcpy(&Atr, fileContent.get(), sizeof(Atr));
    if ((Atr.nMagic1 == ATR_MAGIC1) && (Atr.nMagic2 == ATR_MAGIC2)) {
        info.nResult = ImgError::ATR;

        info.wDensity = (Memory::word)Atr.nSecSizeLo + ((Memory::word)Atr.nSecSizeHi << 8);

        unsigned long lSectorCount;
        lSectorCount = (Memory::dword)Atr.nSecCountLo + ((Memory::dword)Atr.nSecCountHi << 8)
            + ((Memory::dword)Atr.nHugeSecCountLo << 16) + ((Memory::dword)Atr.nHugeSecCountHi << 24);
        lSectorCount >>= 3;

        if (info.wDensity == 256) {
            lSectorCount += 3;
            lSectorCount >>= 1;
        }

        info.wSectors = (Memory::word)lSectorCount;

        return;
    }

    // check if this can be an XFD file
    switch (fileContent.size()) {
        // single density
    case 40 * 18 * 128L:
        info.nResult = ImgError::XFD;
        info.wSectors = 40 * 18;
        info.wDensity = 128;
        break;

        // enhanced density
    case 40 * 26 * 128L:
        info.nResult = ImgError::XFD;
        info.wSectors = 40 * 26;
        info.wDensity = 128;
        break;

        // Indus GT double density
    case 40 * 18 * 256L:
        info.nResult = ImgError::XFD;
        info.wSectors = 40 * 18;
        info.wDensity = 256;
        break;

        // Quad density
    case 80 * 18 * 256L:
        info.nResult = ImgError::XFD;
        info.wSectors = 80 * 18;
        info.wDensity = 256;
        break;
    default:
        // by default, the file is not recognized.
        info.nResult = ImgError::BAD_MAGIC;
        info.wSectors = 0;
        info.wDensity = 0;
    }
}

/*
 * Read a sector from a disk image.
 */
void DiskImage::ReadSector(ImgRWPacket& sector) {
    ImgInfo Info;

    // Get image info to know sector size, disk type,...and call ImageRead
    GetInfo(sector.filePath, Info);

    if (IsError(Info.nResult)) {
        sector.nResult = Info.nResult;
    }
    else {
        sector.nResult = Read(Info, sector);
    }
}

void DiskImage::ReadAbsoluteSector(ImgRWPacket& Sector, unsigned wSector, int& nSize) {
    Sector.wSectorNumber = wSector;
    nSize = 0;

    DiskImage::ReadSector(Sector);

    if (!DiskImage::IsError(Sector.nResult)) {
        if (wSector <= 3) {
            nSize = 128;
        }
        else {
            nSize = Sector.wSectorSize;
        }
    }
}

/*
 * Write a sector to a disk image.
 */
void DiskImage::WriteSector(ImgRWPacket& sector) {
    ImgInfo Info;

    // Get image info to know sector size, disk type,...and call ImageWrite
    GetInfo(sector.filePath, Info);

    if (IsError(Info.nResult)) {
        sector.nResult = Info.nResult;
    }
    else {
        sector.nResult = Write(Info, sector);
    }
}

bool DiskImage::IsError(ImgError err) {
    if (err == ImgError::XFD || err == ImgError::ATR) {
        return false;
    }

    return true;
}

/*
 * Display an error returned by disk image read/write functions.
 * Return true is the status is actually and error.
 */
bool DiskImage::DisplayError(ImgError err) {
    switch (err) {
    case ImgError::XFD:             // XFD Image Detected
    case ImgError::ATR:             // ATR Image Detected
        break;

    case ImgError::BAD_MAGIC:       // File does not have a NICKATARI Signature
        ::g_Application->SendErrorMessageWithID(IDS_ERR_IMG_BAD_MAGIC);
        break;

    case ImgError::FILE_NOT_FOUND:  // File does not exist
        ::g_Application->SendErrorMessageWithID(IDS_ERR_IMG_FILE_NOT_FOUND);
        break;

    case ImgError::OUT_OF_RANGE:    // Sector out of range
        ::g_Application->SendErrorMessageWithID(IDS_ERR_IMG_OUT_OF_RANGE);
        break;

    case ImgError::DISK_ERROR:      // Disk error occured
        ::g_Application->SendErrorMessageWithID(IDS_ERR_IMG_DISK_ERROR);
        break;

    case ImgError::WRITE_PROTECT:   // Denied: Write protect error
        ::g_Application->SendErrorMessageWithID(IDS_ERR_IMG_WRITE_PROTECT);
        break;
    }

    return IsError(err);
}
