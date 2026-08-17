#pragma once

#include "Syntax.h"
#include "Byte.h"
#include "ByteArray.h"

class AtariDiskImage {
public:

    using SectorNumber = unsigned short;    // two bytes, starting with 1
    using SectorCount = unsigned short;     // two bytes
    using SectorSize = unsigned short;      // two bytes

	static void ReadAbsoluteSector(wstring_view diskImageFilePath, SectorNumber wSector, ByteArray& sectorData, SectorSize& sectorSize);
	static void WriteAbsoluteSector(wstring_view diskImageFilePath, SectorNumber wSector, const ByteSequence& sectorData);
};
