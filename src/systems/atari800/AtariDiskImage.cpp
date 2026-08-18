#include "AtariDiskImage.h"

#include "DiskImage.h"


void AtariDiskImage::ReadAbsoluteSector(wstring_view diskImageFilePath, SectorNumber sectorNumber, ByteArray& sectorData, SectorSize& sectorSize) {
    ImgInfo Info;
    ImgRWPacket Sector;

    sectorSize = 0;
    DiskImage::GetInfo(diskImageFilePath, Info);

    if (!DiskImage::IsError(Info.nResult)) {
        Sector.filePath = diskImageFilePath;
        Sector.wSectorNumber = sectorNumber;
        Sector.wSectorSize = Info.wDensity;

        DiskImage::ReadSector(Sector);

        if (!DiskImage::IsError(Sector.nResult)) {
            std::memcpy(sectorData.get(), Sector.cSectorData.get(), Sector.wSectorSize);

            if (sectorNumber <= 3) {
                sectorSize = 128;
            }
            else {
                sectorSize = Sector.wSectorSize;
            }
        }
    }
}

/*
* Write an absolute sector (128 or 256 bytes).
*/
void AtariDiskImage::WriteAbsoluteSector(wstring_view diskImageFilePath, SectorNumber sectorNumber, const ByteSequence& sectorData) {
    ImgInfo Info;
    ImgRWPacket Sector;

    DiskImage::GetInfo(diskImageFilePath, Info);

    if (!DiskImage::IsError(Info.nResult)) {
        Sector.filePath = diskImageFilePath;
        Sector.wSectorNumber = sectorNumber;
        Sector.wSectorSize = Info.wDensity;

        sectorData.copyTo(Sector.cSectorData.get(), 0, Sector.cSectorData.size(), 0, Sector.wSectorSize); 

        DiskImage::WriteSector(Sector);
    }
}
