#include "InputStream.h"
#include "MemoryBlock.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include "Strings.h"
#include "C64.h"
#include <set>

C64::C64(const ComputerSystemTypeInfo& computerSystemTypeInfo) : ComputerSystem(computerSystemTypeInfo) {
    returnCharacter = 0x0d;

    supportedFileTypes = std::vector <FileType>{
        FileType::RAW_FILE,
        FileType::EXECUTABLE_FILE,
        //FileType::ROM_IMAGE_FILE,
        //FileType::CASSETTE_IMAGE_FILE,
        //FileType::DISK_IMAGE_EXECUTABLE_FILE,
        //FileType::DISK_IMAGE_BOOT_SECTORS,
        //FileType::DISK_IMAGE_SECTORS 
    };
}

bool C64::IsBaseAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
    0x0200, /*DUMMY EXAPLE*/
    };

    if (addresses.contains(address)) {
        return true;
    }

    return ((address & 0xFF00) == 0); // TODO: Why are these base addresses?
}

bool C64::IsVectorAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
        0x0200, /*DUMMY EXAMPLE*/
    };

    return (addresses.contains(address));
}

FileType C64::GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const {
    //// DOS 2.5 ($ffff) or SDX header ($fffa, $fffe)?
    //const FileHeader wHeader = static_cast<FileHeader>(content.at(0) | (content.at(1) << 8));
    //if (wHeader == FileHeader::ATARI_BINARY || wHeader == FileHeader::SDX_FIXED_BLK || wHeader == FileHeader::SDX_RELOC_BLK) {
    //    return FileType::EXECUTABLE_FILE;
    //}

    //// 4k, 8k or 16k ROM image?
    //if (fileSize == SIZE_4K || fileSize == SIZE_8K || fileSize == SIZE_16K) {
    //    return FileType::ROM_IMAGE_FILE;
    //}

    //if (fileSize == SIZE_4K_CAR || fileSize == SIZE_8K_CAR || fileSize == SIZE_16K_CAR) {
    //    return FileType::ROM_IMAGE_FILE;
    //}

    //// Cassette image?
    //if (content.at(0) == 'F' && content.at(1) == 'U' && content.at(2) == 'J' && content.at(3) == 'I') {
    //    return FileType::CASSETTE_IMAGE_FILE;
    //}

    //// Disk Image (SD, MD) without (XFD) and with (ATR) header.
    //if (fileSize == 92160L || fileSize == 133120L || fileSize == 92176L || fileSize == 133136L ||
    //    ((content.at(0) == (byte)0x96) && (content.at(1) == (byte)0x02) && fileSize <= 133136L)) {
    //    return FileType::DISK_IMAGE_BOOT_SECTORS;
    //}

    return FileType::UNKNOWN_FILE;
}

void C64::ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    Memory::address wAddr;

    if (fileSize > PRG_MAX_SIZE) {
        throw IOException(String::Format(L"File size of {0} bytes exceeds the maximum file size of executable files on C64", std::to_wstring(fileSize)));
    }
    constexpr Memory::size headerSize = sizeof(wAddr);
    inputStream.Read(&wAddr, headerSize);

    const Memory::size size = fileSize - headerSize;
    const auto& segment = segmentListInserter.InsertSegment();
    segment->wBegin = wAddr;
    segment->wEnd = wAddr + size - 1;
    segment->CreateMemoryBlockFromFile(size, inputStream);
};

void C64::WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool writeHeader, OutputStream& outputStream) const {
    Memory::address wAddr;
    constexpr Memory::size headerSize = sizeof(wAddr);
    SEGMENT_INDEX segmentIndex;
    if (firstSegmentIndex == NO_SEGMENT_INDEX) {
        if (segmentList.GetCount() > 1) {
            throw IOException(L"Executable files on C64 can only have one segment");
        }
        segmentIndex = 0;

    }
    else {
        segmentIndex = firstSegmentIndex;
    }
    const auto& segment = segmentList.GetConstSegment(segmentIndex);
    wAddr = segment->wBegin;
    outputStream.Write(&wAddr, headerSize);
    segment->memoryBlock.WriteData(outputStream);
}