/*
 * DiskImage.h
 * API to read/write sectors in a disk image.
 */
#pragma once

#include "Byte.h"
#include "ByteArray.h"
#include "Syntax.h"
#include "Word.h"

 /*
 ** Error codes written in nResult field.
 */
enum class ImgError {
    XFD = 0,                // XFD Image Detected
    ATR = 1,                // ATR Image Detected
    BAD_MAGIC = -1,         // File does not have a NICKATARI Signature
    FILE_NOT_FOUND = -2,    // File can not be opened
    OUT_OF_RANGE = -3,      // Sector out of range
    DISK_ERROR = -4,        // Disk error occured
    WRITE_PROTECT = -5      // Denied: Write protect error
};

/*
** Class to get information about a .XFD/.ATR image disk.
*/
class ImgInfo {
public:
    ImgError nResult;       // Operation status
    word wDensity;          // Verified bytes-per-sector in image
    word wSectors;          // Verified number of sectors in image
    byte cWriteProtect;     // true if the image is write-protected
};

/*
** Structure to read/write a sector to an.XFD/.ATR image disk.
*/
class ImgRWPacket {
public:

    ImgRWPacket(): wSectorNumber(0), cSectorData(ByteArray(256)), wSectorSize(0), nResult(ImgError::DISK_ERROR) {

    }
    wstring filePath;        // File path of the .XFD/.ATR disk image  // TODO: Call this diskPath or ImagePath
    word wSectorNumber;     // Sector number to read or write, starting with 1
    ByteArray cSectorData;  // Area for transfer of data, read/write
    word wSectorSize;       // Size of sector to read or bytes written
    ImgError nResult;       // Operation status
};

class DiskImage {
public:
    static void GetInfo(wstring_view filePath, ImgInfo& info);
    static void ReadSector(ImgRWPacket& sector);
    static void ReadAbsoluteSector(ImgRWPacket& Sector, unsigned wSector, int& nSize);
    static void WriteSector(ImgRWPacket& sector);
    static bool IsError(ImgError err);
    static bool DisplayError(ImgError err);

private:
    /*
    ** Magic numbers for ATR disks.
    */
    static const byte ATR_MAGIC1 = 0x96;
    static const byte ATR_MAGIC2 = 0x02;

    /*
    ** ATR header.
    */
    typedef struct tag_ATRHeader {
        byte nMagic1;
        byte nMagic2;
        byte nSecCountLo;
        byte nSecCountHi;
        byte nSecSizeLo;
        byte nSecSizeHi;
        byte nHugeSecCountLo;
        byte nHugeSecCountHi;
        byte nReserved[8];
    } ATRHeader;

    /*
    ** Seek to a specific sector in the file.
    */
    static ImgError Seek(FILE* fd, const ImgInfo& info, const ImgRWPacket& sector);

    /*
    ** Read an absolute sector (128 or 256 bytes).
    */
    static ImgError Read(const ImgInfo& info, ImgRWPacket& sector);

    /*
    ** Write an absolute sector (128 or 256 bytes).
    */
    static ImgError Write(const ImgInfo& info, const ImgRWPacket& sector);
};
