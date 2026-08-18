#include "Atari5200.h"

#include "FileInputStream.h"
#include "FileType.h"
#include "OutputStream.h"
#include "Segment.h"
#include "SegmentListInserter.h"
#include "Strings.h"
#include "Workspace.h"
#include <set>

Atari5200::Atari5200(const ComputerSystemTypeInfo& computerSystemTypeInfo) : ComputerSystem(computerSystemTypeInfo) {
    returnCharacter = 0x9b;

    supportedFileTypes = std::vector<FileType>{
        FileType::RAW_FILE,
        FileType::ROM_IMAGE_FILE
    };
}

bool Atari5200::IsBaseAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
        0x0005, /*SDLSTL*/
        0x0200, /*VIMIRQ*/
        0x0202, /*VVBLKI*/
        0x0204, /*VVBLKD*/
        0x0206, /*VDLI*/
        0x0208, /*VKEYBD*/
        0x020A, /*VKEYPAD*/
        0x020C, /*VBREAK*/
        0x020E, /*VBRK*/
        0x0210, /*VSERIN*/
        0x0212, /*VSEROR*/
        0x0214, /*VSEROC*/
        0x0216, /*VTIMR1*/
        0x0218, /*VTIMR2*/
        0x021A, /*VTIMR4*/
        0xD402  /*DLISTL*/
    };

    if (addresses.contains(address)) {
        return true;
    }

    return (((address & 0xFF00) == 0) && (address > 0x10));
}

/*
** retruns true if the address is a vector address
*/
bool Atari5200::IsVectorAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
        0x0200, /*VIMIRQ*/
        0x0202, /*VVBLKI*/
        0x0204, /*VVBLKD*/
        0x0206, /*VDLI*/
        0x0208, /*VKEYBD*/
        0x020A, /*VKEYPAD*/
        0x020C, /*VBREAK*/
        0x020E, /*VBRK*/
        0x0210, /*VSERIN*/
        0x0212, /*VSEROR*/
        0x0214, /*VSEROC*/
        0x0216, /*VTIMR1*/
        0x0218, /*VTIMR2*/
        0x021A  /*VTIMR4*/
    };

    return addresses.contains(address);
}

bool Atari5200::IsDisplayListVectorAddress(const Memory::address address) const {
    return (address == 0x0005 || address == 0xD402);
}

FileType Atari5200::GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const {
    // See https://github.com/atari800/atari800/blob/master/DOC/cart.txt
    // Bank-switched cartridges and .CAR files are currently not yet supported.
    // 4k, 8k, 16k, 32k or 40k ROM?
    if (fileSize == 0x1000L || fileSize == 0x2000L || fileSize == 0x4000 || fileSize == 0x8000L || fileSize == 0xa000L) {
        return FileType::ROM_IMAGE_FILE;
    }

    return FileType::UNKNOWN_FILE;
}

void Atari5200::ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {

    // Check that the file length is less than 32K.
    if (fileSize > 32768L) {
        throw IOException(L"Only 32k ROMs are supported");
    }

    const auto memorySize = Memory::size(fileSize);

    // Read segment data.
    const auto& segment = segmentListInserter.InsertSegment();

    segment->wBegin = 0xC000 - memorySize;
    segment->wEnd = 0xBFFF;
    segment->bBinary = true;
    segment->CreateMemoryBlockFromFile(memorySize, inputStream);

    // Extract title from data and convert it to ASCII.
    // There are 20 bytes at the end of the ROM, followed by two 16-bit vectors
    // See https://atarihq.com/danb/files/5200BIOS.txt
    // FE63 : A2 13    ldx  #$13;
    // FE65 : BD D4 FE lda  $FED4, x; Read copywrite message
    // FE68 : 9D 94 3C sta  $3C94, x; Write to display memory
    // FE6B : BD E8 BF lda  $BFE8, x; Read cart title from cartridge
    // FE6E : 9D 80 3C sta  $3C80, x; Write to display memory
    // FE71 : CA       dex;
    // FE72 : 10 F1    bpl  $FE65;
    const gsl::not_null<MemoryBlock*> lpMemoryBlock = &segment->memoryBlock;
    const Memory::offset titleOffset = lpMemoryBlock->GetSize() - 24;
    constexpr Memory::size titleSize = 20;
    segment->SetType(titleOffset, MemoryType::SBYTE, titleSize);
    const Memory::offset vectorOffset = lpMemoryBlock->GetSize() - 4;
    constexpr Memory::size vectorSize = 4;
    segment->SetType(vectorOffset, MemoryType::LABEL, vectorSize);

    // Convert Atari Screen Code to ASCII
    for (Memory::offset i = 0; i < titleSize; i++) {
        auto b = segment->GetData(titleOffset + i);

        if (b < 64) {
            b += 32;
        }
        else if (b < 96) {
            b -= 64;
        }
        else if ((b >= 128) && (b < 128 + 64)) {
            b += 32;
        }
        else if ((b >= 128 + 64) && (b < 128 + 96)) {
            b -= 64;
        }
        const wchar_t c = std::toupper(b);
        segment->szTitle.append(&c, 1);
    }

    segment->szTitle = String::Trim(segment->szTitle);
}
