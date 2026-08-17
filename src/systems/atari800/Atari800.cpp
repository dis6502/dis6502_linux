#include "Atari800.h"

#include "../../Debug.h"
#include "../../FileHeader.h"
#include "../../String.h"
#include "../../Fixup.h"
#include "../../InputStream.h"
#include "../../OutputStream.h"
#include "../../Segment.h"
#include "../../SegmentList.h"
#include "../../SegmentListInserter.h"
#include <set>
#include <sstream>


Atari800::Atari800(const ComputerSystemTypeInfo& computerSystemTypeInfo) : ComputerSystem(computerSystemTypeInfo) {
    returnCharacter = 0x9B;

    supportedFileTypes = std::vector<FileType>{
        FileType::RAW_FILE,
        FileType::EXECUTABLE_FILE,
        FileType::ROM_IMAGE_FILE,
        FileType::CASSETTE_IMAGE_FILE,
        FileType::DISK_IMAGE_EXECUTABLE_FILE,
        FileType::DISK_IMAGE_BOOT_SECTORS,
        FileType::DISK_IMAGE_SECTORS };
}

bool Atari800::IsBaseAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
    0x0200, /*VDSLST*/
    0x0202, /*VPRCED*/
    0x0204, /*VINTER*/
    0x0206, /*VBREAK*/
    0x0208, /*VKEYBD*/
    0x020A, /*VSERIN*/
    0x020C, /*VSEROR*/
    0x020E, /*VSEROC*/
    0x0210, /*VTIMR1*/
    0x0212, /*VTIMR2*/
    0x0214, /*VTIMR4*/
    0x0216, /*VIMIRQ*/
    0x0222, /*VVBLKI*/
    0x0224, /*VVBLKD*/
    0x0226, /*CDTMA1*/
    0x0228, /*CDTMA2*/
    0x0230, /*SDLSTL*/
    0x0236, /*BRKKY*/
    0x0238, /*VPIRQ*/
    0x02E0, /*RUNAD*/
    0x02E2, /*INITAD*/
    0x02E5, /*MEMTOP*/
    0x02E7, /*MEMLO*/
    0x0304, /*DBUFLO*/
    0x0344, /*ICBAL0*/
    0x0354, /*ICBAL1*/
    0x0364, /*ICBAL2*/
    0x0374, /*ICBAL3*/
    0x0384, /*ICBAL4*/
    0x0394, /*ICBAL5*/
    0x03A4, /*ICBAL6*/
    0x03B4, /*ICBAL7*/
    0xD402  /*DLISTL*/
    };

    if (addresses.contains(address)) {
        return true;
    }

    return ((address & 0xFF00) == 0);
}

bool Atari800::IsVectorAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
        0x0200, /*VDSLST*/
        0x0202, /*VPRCED*/
        0x0204, /*VINTER*/
        0x0206, /*VBREAK*/
        0x0208, /*VKEYBD*/
        0x020A, /*VSERIN*/
        0x020C, /*VSEROR*/
        0x020E, /*VSEROC*/
        0x0210, /*VTIMR1*/
        0x0212, /*VTIMR2*/
        0x0214, /*VTIMR4*/
        0x0216, /*VIMIRQ*/
        0x0222, /*VVBLKI*/
        0x0224, /*VVBLKD*/
        0x0226, /*CDTMA1*/
        0x0228, /*CDTMA2*/
        0x0236, /*BRKKY*/
        0x0238, /*VPIRQ*/
        0x02E0, /*RUNAD*/
        0x02E2, /*INITAD*/
    };

    return addresses.contains(address);
}

bool Atari800::IsDisplayListVectorAddress(const Memory::address address) const {
    return (address == 0x0230 || address == 0xD402);
}

FileType Atari800::GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const {
    // DOS 2.5 ($ffff) or SDX header ($fffa, $fffe)?
    const FileHeader wHeader = static_cast<FileHeader>(content.at(0) | (content.at(1) << 8));
    if (wHeader == FileHeader::ATARI_BINARY || wHeader == FileHeader::SDX_FIXED_BLK || wHeader == FileHeader::SDX_RELOC_BLK) {
        return FileType::EXECUTABLE_FILE;
    }

    // 4k, 8k or 16k ROM image?
    if (fileSize == SIZE_4K || fileSize == SIZE_8K || fileSize == SIZE_16K) {
        return FileType::ROM_IMAGE_FILE;
    }

    if (fileSize == SIZE_4K_CAR || fileSize == SIZE_8K_CAR || fileSize == SIZE_16K_CAR) {
        return FileType::ROM_IMAGE_FILE;
    }

    // Cassette image?
    if (content.at(0) == 'F' && content.at(1) == 'U' && content.at(2) == 'J' && content.at(3) == 'I') {
        return FileType::CASSETTE_IMAGE_FILE;
    }

    // Disk Image (SD, MD) without (XFD) and with (ATR) header.
    if (fileSize == 92160L || fileSize == 133120L || fileSize == 92176L || fileSize == 133136L ||
        ((content.at(0) == 0x96u) && (content.at(1) == 0x02u) && fileSize <= 133136L)) {
        return FileType::DISK_IMAGE_BOOT_SECTORS;
    }

    return FileType::UNKNOWN_FILE;
}

/*
** allocate a 1 byte segment to mark it as being used
*/
bool Atari800::FakeSegment(Segment& segment) const {
    // allocate memory for data and for byte type.
    segment.CreateMemoryBlockWithSize(1);

    return true;
}

void Read(InputStream& inputStream, void* buffer, size_t size, FileIO::FILE_SIZE& bytesRemaining) {
    if (size > bytesRemaining) {
        throw IOException(L"Computed remaining length of stream of " + std::to_wstring(bytesRemaining) + L" is smaller than requested amount of " + std::to_wstring(size) + L" to read");
    }
    inputStream.Read(buffer, size);
    bytesRemaining -= size;
}
/*
** load a fix-ups from a SDX binary file
*/
bool Atari800::LoadFixUps(Segment& segment, InputStream& inputStream, FileIO::FILE_SIZE& bytesRemaining) const {
    byte bByte = 0;
    int offset = 0;

    constexpr Memory::size bufferSize = 8192;
    ByteArray buffer(bufferSize); // unknown size so allocate enough for fix-up data
    // read byte after byte until we get an end code (0xFC)
    Read(inputStream, &bByte, sizeof(bByte), bytesRemaining);
    buffer.setAt(offset++, bByte);

    while ((bByte != static_cast<byte>(FixupType::END)) && (offset < (bufferSize - 3))) {
        // some bytes have parameters. Read them
        if (bByte == static_cast<byte>(FixupType::SET_BLOCK_NUM)) {
            Read(inputStream, &bByte, sizeof(bByte), bytesRemaining);
            buffer.setAt(offset++, bByte);
        }
        else if (bByte == static_cast<byte>(FixupType::SET_BLOCK_ADDR)) {
            Read(inputStream, &bByte, sizeof(bByte), bytesRemaining);
            buffer.setAt(offset++, bByte);
            Read(inputStream, &bByte, sizeof(bByte), bytesRemaining);
            buffer.setAt(offset++, bByte);
        }

        // read next byte
        Read(inputStream, &bByte, sizeof(bByte), bytesRemaining);
        buffer.setAt(offset++, bByte);
    }

    // allocate memory for data and for byte type.
    segment.wBegin = 0;
    segment.wEnd = offset - 1;
    segment.CreateMemoryBlockFromBeginToEnd();
    segment.SetData(0, buffer, 0, offset);

    return true;
}
using SDXSymbol = char[dis_k::SDX_SYMBOL_LEN];

// Read and SDX symbol name from the input stream.
// Symbols have fixed length in the file, but a re returned without trailing spaces.
wstring ReadSDXSymbol(InputStream& inputStream, FileIO::FILE_SIZE& bytesRemaining) {
    SDXSymbol sdxSymbol;
    Read(inputStream, sdxSymbol, sizeof(sdxSymbol), bytesRemaining);
    wstring result;
    for (int i = 0; i < dis_k::SDX_SYMBOL_LEN; i++) {
        const auto c = gsl::at(sdxSymbol, i);
        if (c != ' ') {
            result.append(1, c);
        }
        else {
            break;
        }
    }
    return result;
}

void WriteSDXSymbol(wstring_view string, OutputStream& outputStream) {
    SDXSymbol sdxSymbol = { 32,32,32,32,32,32,32,32 }; // Initialize with spaces

    constexpr auto maxLength = sizeof(sdxSymbol);
    const auto length = string.length();
    if (length > maxLength) {
        throw IOException(String::Format(L"Length of SDX symbol '{0}' exceeds maximum length {1}", string, std::to_wstring(maxLength)));
    }
    for (size_t i = 0; i < length; i++) {
        gsl::at(sdxSymbol,i) = string.at(i);
    }

    outputStream.Write(sdxSymbol, maxLength);
}

void ReadData(InputStream& inputStream, Segment& segment, FileIO::FILE_SIZE& bytesRemaining) {
    ByteArray byteArray(segment.GetSize());
    Read(inputStream, byteArray.get(), byteArray.size(), bytesRemaining);
    segment.SetData(0, byteArray, 0, byteArray.size());
}

void Atari800::ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    FileHeader wHeader;
    Memory::size nSize;

    auto bytesRemaining = fileSize;

    // Check that the file begins with a known header
    Read(inputStream, &wHeader, sizeof(wHeader), bytesRemaining);

    if ((wHeader != FileHeader::ATARI_BINARY) && (wHeader != FileHeader::SDX_FIXED_BLK) &&
        (wHeader != FileHeader::SDX_SYM_REQUIRED) && (wHeader != FileHeader::SDX_SYM_DEFINED) &&
        (wHeader != FileHeader::SDX_FIX_UP_BLK) && (wHeader != FileHeader::SDX_RELOC_BLK)) {
        throw IOException(L"Unsupported file header " + to_wstring(wHeader));
    }

    // read all segments
    auto bFirstSegment = true;
    auto wPreviousHeader = FileHeader::RAW;
    Memory::word wBegin = 0xFFFF;

    while (bytesRemaining) {
        const auto& segment = segmentListInserter.InsertSegment();

        // read header depending on the type
        switch (wHeader) {
        case FileHeader::SDX_RELOC_BLK: {
            // read block number definition.
            byte bSDXBlockNumber = 0;
            Read(inputStream, &bSDXBlockNumber, sizeof(bSDXBlockNumber), bytesRemaining);

            // read control byte
            byte bSDXControlByte = 0;
            Read(inputStream, &bSDXControlByte, sizeof(bSDXControlByte), bytesRemaining);

            // read offset from the begining of the memory block.
            Read(inputStream, &wBegin, sizeof(wBegin), bytesRemaining);

            // read size to read or allocate.
            Memory::word wEnd = 0;
            Read(inputStream, &wEnd, sizeof(wEnd), bytesRemaining);
            wEnd = wBegin + wEnd - 1;

            // read segment data.

            segment->SetHeader(wHeader);
            segment->wBegin = wBegin;
            segment->wEnd = wEnd;
            segment->bSDXBlockNumber = bSDXBlockNumber;
            segment->bSDXControlByte = bSDXControlByte;
            segment->bBinary = false;

            // is there any data after header
            if (segment->IsSDXRelocBlkWithData()) {
                // create segment of required size.
                segment->CreateMemoryBlockFromBeginToEnd();

                // read segment from file.
                ReadData(inputStream, *segment, bytesRemaining);
                segment->bBinary = true;
            }
            else {
                // No file data follows (bSDXControlByte's high bit is
                // set) - this is a "reserve N bytes of RAM" block (see
                // the RamBlk case in Segment::ToString()/GetSDXBlockType),
                // e.g. for BSS-style uninitialized storage the loader
                // reserves without any bytes to actually read. wBegin/
                // wEnd above already declare that real, meaningful size
                // (unlike e.g. SDX_SYM_DEFINED's point-definition, which
                // has no size and legitimately wants FakeSegment's 1-byte
                // placeholder). Using FakeSegment here instead left
                // Segment::GetSize() (wEnd-wBegin+1, often many bytes)
                // completely out of sync with the actual 1-byte buffer it
                // allocated - any UI code trusting GetSize() to bound its
                // reads (e.g. the Memory Inspector) would then read past
                // that 1 byte and crash. CreateMemoryBlockFromBeginToEnd
                // allocates the declared size for real, zero-initialized,
                // keeping it consistent with GetSize().
                segment->CreateMemoryBlockFromBeginToEnd();
            }
            break;
        }

        case FileHeader::SDX_FIX_UP_BLK: {
            // read block number to fix-up.
            byte bSDXBlockNumber = 0;
            Read(inputStream, &bSDXBlockNumber, sizeof(bSDXBlockNumber), bytesRemaining);

            // read length (never seen values other than 0 !).
            Memory::word wSDXFixUpSize = 0;
            Read(inputStream, &wSDXFixUpSize, sizeof(wSDXFixUpSize), bytesRemaining);

            // read segment data.
            segment->SetHeader(wHeader);
            segment->wBegin = 0; // wEnd will be set by LoadFixUps
            segment->bSDXBlockNumber = bSDXBlockNumber;
            segment->wSDXFixUpSize = wSDXFixUpSize;
            segment->bBinary = false;
            LoadFixUps(*segment, inputStream, bytesRemaining);
            break;
        }

        case FileHeader::SDX_SYM_REQUIRED: {
            // read symbol name.
            auto sdxSymbol = ReadSDXSymbol(inputStream, bytesRemaining);

            // read length (never seen values other than 0 !).
            Memory::word wSDXFixUpSize = 0;
            Read(inputStream, &wSDXFixUpSize, sizeof(wSDXFixUpSize), bytesRemaining);

            // read segment data.
            segment->SetHeader(wHeader);
            segment->wBegin = 0; // wEnd will be set by LoadFixUps
            segment->wSDXFixUpSize = wSDXFixUpSize;
            segment->bBinary = false;
            segment->szSDXSymbol = sdxSymbol;

            LoadFixUps(*segment, inputStream, bytesRemaining);

            break;
        }

        case FileHeader::SDX_SYM_DEFINED: {
            // read block number to fix-up.
            byte bSDXBlockNumber = 0;
            Read(inputStream, &bSDXBlockNumber, sizeof(bSDXBlockNumber), bytesRemaining);

            // read length (never seen values other than 0 !).
            Read(inputStream, &wBegin, sizeof(wBegin), bytesRemaining);

            // read symbol name.
            auto sdxSymbol = ReadSDXSymbol(inputStream, bytesRemaining);

            // read segment data.
            segment->SetHeader(wHeader);
            segment->wBegin = wBegin;
            segment->wEnd = wBegin;
            segment->bSDXBlockNumber = bSDXBlockNumber;
            segment->bBinary = false;
            segment->szSDXSymbol = sdxSymbol;

            // mark segment as being used by allocating a 1 byte buffer
            FakeSegment(*segment);
            break;
        }

        case FileHeader::ATARI_BINARY:
        case FileHeader::SDX_FIXED_BLK:
            // read segment start address.
            if (wBegin == 0xFFFF) {
                Read(inputStream, &wBegin, sizeof(wBegin), bytesRemaining);
            }

            wPreviousHeader = wHeader;
            [[fallthrough]]; // fallthrough is explicit

        default: {
            // read segment end address.
            const FileHeader fhBegin = static_cast<FileHeader>(wBegin);

            if ((!bFirstSegment) && ((fhBegin == FileHeader::ATARI_BINARY) || (fhBegin == FileHeader::SDX_FIXED_BLK))) {
                wHeader = fhBegin;
                Read(inputStream, &wBegin, sizeof(wBegin), bytesRemaining);
            }
            else {
                wHeader = wPreviousHeader;
            }
            Memory::word wEnd = 0;
            Read(inputStream, &wEnd, sizeof(wEnd), bytesRemaining);

            if (wEnd < wBegin) {
                throw IOException(L"Segement end address is lower than segment start address");
            }

            // check data size.
            nSize = wEnd - wBegin + 1;
            if (nSize > bytesRemaining) {
                throw IOException(wstringstream() << "Stream has " << bytesRemaining << " bytes left and is too short for segment of size" << nSize);
            }

            //Debug::Log(String::Format("Starting segment at offset {0}: header={1}, begin={2}, end={3}, size {4}, bytes remaining {5}",
            //    std::to_hex_string(position), std::to_hex_string((uintmax_t)wHeader),
            //    std::to_hex_string((uintmax_t)wBegin), std::to_hex_string((uintmax_t)wEnd), std::to_hex_string((uintmax_t)nSize),
            //    std::to_hex_string(bytesRemaining)));

            // read segment data.
            segment->SetHeader(wHeader);
            segment->wBegin = wBegin;
            segment->wEnd = wEnd;
            segment->bBinary = true;
            segment->CreateMemoryBlockFromFile(nSize, inputStream);
            bytesRemaining -= nSize; // TODO: Introduce bounded InputStream

            // change segment type if segment is 02E0 or 02E2.
            if (((wBegin == 0x2E0) && ((nSize == 2) || (nSize == 4))) || ((wBegin == 0x2E2) && (nSize == 2))) {
                segment->SetType(0, MemoryType::LABEL, nSize);
            }

            wPreviousHeader = wHeader;
            break;
        }
        }

        Debug::Log(segment->ToString() + L" - remaining " + std::to_hex_string(bytesRemaining));

        if (bytesRemaining == 0) {
            return;
        }

        // read next segment header (assume it is the begin address with no magic header).
        //position = inputStream.GetPosition();
        Read(inputStream, &wBegin, sizeof(wBegin), bytesRemaining);

        // if the Begin address is a magic header, transfer it to Header
        const auto fhBegin = static_cast<FileHeader>(wBegin);
        if ((fhBegin == FileHeader::ATARI_BINARY) || (fhBegin == FileHeader::SDX_FIXED_BLK) ||
            (fhBegin == FileHeader::SDX_SYM_REQUIRED) || (fhBegin == FileHeader::SDX_SYM_DEFINED) ||
            (fhBegin == FileHeader::SDX_FIX_UP_BLK) || (fhBegin == FileHeader::SDX_RELOC_BLK)) {
            wHeader = fhBegin;
            wBegin = 0xFFFF;
        }
        else {
            wHeader = wPreviousHeader;
        }

        bFirstSegment = false;
    }
}

void Atari800::WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool writeHeader, OutputStream& outputStream) const {
    const SEGMENT_INDEX nFirstSegment = (firstSegmentIndex != NO_SEGMENT_INDEX) ? firstSegmentIndex : 0;
    const SEGMENT_INDEX nLastSegment = (firstSegmentIndex != NO_SEGMENT_INDEX) ? firstSegmentIndex + 1 : segmentList.GetCount();
    bool bWriteHeaderLocal = writeHeader;

    if (firstSegmentIndex == NO_SEGMENT_INDEX) {
        bWriteHeaderLocal = true;
    }

    FileHeader wLastHeader = FileHeader::RAW;
    Memory::word wSize = 0;

    for (int i = nFirstSegment; i < nLastSegment; i++) {
        const auto& segment = segmentList.GetConstSegment(i);
        if (!segment->IsEmpty()) {
            bool hasData = true;

            if (bWriteHeaderLocal) {
                const auto header = segment->GetHeader();
                if (header != wLastHeader || header != FileHeader::ATARI_BINARY) {
                    outputStream.Write(&header, sizeof(header));
                }

                switch (header) {
                case FileHeader::SDX_RELOC_BLK:
                    outputStream.Write(&segment->bSDXBlockNumber, sizeof(segment->bSDXBlockNumber));
                    outputStream.Write(&segment->bSDXControlByte, sizeof(segment->bSDXControlByte));
                    outputStream.Write(&segment->wBegin, sizeof(segment->wBegin));

                    wSize = segment->GetSize();
                    outputStream.Write(&wSize, sizeof(wSize));

                    hasData = segment->IsSDXRelocBlkWithData();
                    break;

                case FileHeader::SDX_FIX_UP_BLK:
                    outputStream.Write(&segment->bSDXBlockNumber, sizeof(segment->bSDXBlockNumber));
                    outputStream.Write(&segment->wSDXFixUpSize, sizeof(segment->wSDXFixUpSize));
                    break;

                case FileHeader::SDX_SYM_REQUIRED:
                    WriteSDXSymbol(segment->szSDXSymbol, outputStream);
                    outputStream.Write(&segment->wSDXFixUpSize, sizeof(segment->wSDXFixUpSize));
                    break;

                case FileHeader::SDX_SYM_DEFINED:
                    outputStream.Write(&segment->bSDXBlockNumber, sizeof(segment->bSDXBlockNumber));
                    outputStream.Write(&segment->wBegin, sizeof(segment->wBegin));
                    WriteSDXSymbol(segment->szSDXSymbol, outputStream);
                    break;

                case FileHeader::ATARI_BINARY:
                case FileHeader::SDX_FIXED_BLK:
                default:
                    outputStream.Write(&segment->wBegin, sizeof(segment->wBegin));
                    outputStream.Write(&segment->wEnd, sizeof(segment->wEnd));
                    break;
                }

                wLastHeader = header;
            }
            if (hasData) {
                segment->memoryBlock.WriteData(outputStream);
            }

        }
    }
}

void Atari800::ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    constexpr Memory::address BASE_4K = 0xB000;
    constexpr Memory::address END_4K = 0xBFFF;
    constexpr Memory::address BASE_8K = 0xA000;
    constexpr Memory::address END_8K = 0xBFFF;
    constexpr Memory::address BASE_16K = 0x8000;
    constexpr Memory::address END_16K = 0xBFFF;

    Memory::address wBegin = 0, wEnd = 0;

    // TODO: Code contains too many casts

    // Get file size.
    auto bytesRemaining = fileSize;

    if ((bytesRemaining == SIZE_8K_CAR) || (bytesRemaining == SIZE_16K_CAR)) {
        byte carHeader[CAR_HEADER_SIZE];
        inputStream.Read(carHeader, CAR_HEADER_SIZE);

        if (memcmp(carHeader, "CART", 4) != 0) {
            throw IOException(L"Invalid stream header. Stream is not a CART stream");
        }
        bytesRemaining -= CAR_HEADER_SIZE;
    }


    // Read only 4K, 8K and 16K cartriges.
    switch (bytesRemaining) {
    case SIZE_4K:
        wBegin = BASE_4K;
        wEnd = END_4K;
        break;
    case SIZE_8K:
        wBegin = BASE_8K;
        wEnd = END_8K;
        break;
    case SIZE_16K:
        wBegin = BASE_16K;
        wEnd = END_16K;
        break;
    default:
        throw IOException(wstringstream() << "Unsupported cartridge size " << bytesRemaining);
    }

    // read segment data.
    const auto& segment = segmentListInserter.InsertSegment();

    segment->wBegin = wBegin;
    segment->wEnd = wEnd;
    segment->bBinary = true;
    const Memory::size wSize = segment->wEnd - segment->wBegin + 1;
    segment->CreateMemoryBlockFromFile(wSize, inputStream);

    // change type of the last 6 bytes
    // TODO: Put this into a method for all Atari ROMs
    Memory::offset offset = (Memory::offset)bytesRemaining - 6;
    segment->SetType(offset++, MemoryType::LABEL);
    segment->SetType(offset++, MemoryType::LABEL);
    segment->SetType(offset++, MemoryType::BYTE);
    segment->SetType(offset++, MemoryType::BYTE);
    segment->SetType(offset++, MemoryType::LABEL);
    segment->SetType(offset++, MemoryType::LABEL);
}

typedef struct CAS_HEADER {
    char name[4];
    short length;
    short unused;
} CAS_HEADER;

typedef struct CAS_SEGMENT {
    char unused;
    byte segmentCount;
    Memory::address loadAddress;
    Memory::address initAddress;
} CAS_SEGMENT;

void Atari800::ReadCassetteFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    static constexpr char FUJI[4] = { 'F','U','J','I' };
    static constexpr Memory::size CAS_HEADER_SIZE = 4;

    byte casHeader[CAS_HEADER_SIZE];
    int expectedSegmentCount = 0;
    int iOffset = 0;

    // Check that the file begins with 'FUJI'.
    inputStream.Read(casHeader, CAS_HEADER_SIZE);
    if (memcmp(casHeader, FUJI, CAS_HEADER_SIZE) != 0) {
        throw IOException(L"Invalid file header. Stream is not a FUJI stream");
    }

    auto bytesRemaining = fileSize - CAS_HEADER_SIZE;

    // Skip title of software
    short iSize;
    inputStream.Read(&iSize, sizeof(iSize));
    bytesRemaining -= iSize;

    inputStream.Skip(2);
    bytesRemaining -= 2;


    // Read all the data chunks
    bool firstSegment = true;
    byte* szBuf = nullptr;

    while (bytesRemaining > sizeof(CAS_HEADER)) {
        const auto& segment = segmentListInserter.InsertSegment();

        // read a header
        CAS_HEADER header;
        inputStream.Read(&header, sizeof(header));

        bytesRemaining -= sizeof(header);

        static constexpr char DATA[4] = { 'd','a','t','a' };
        if (memcmp(header.name, DATA, sizeof(header.name)) == 0) {
            // save previous segment
            if ((expectedSegmentCount == 0) && (szBuf != nullptr) && (iOffset > 0)) {
                const CAS_SEGMENT* cassetteSegment = (CAS_SEGMENT*)szBuf;

                // allocate memory for data and for byte type.
                segment->CreateMemoryBlockWithSize(iOffset);
                const ByteArray buffer = ByteArray(szBuf, segment->GetSize());
                segment->SetData(0, buffer, 0, buffer.size());

                if (firstSegment == true) {
                    segment->SetType(0, MemoryType::BYTE);
                    segment->SetType(1, MemoryType::BYTE);
                    segment->SetType(2, MemoryType::LABEL);
                    segment->SetType(3, MemoryType::LABEL);
                    segment->SetType(4, MemoryType::LABEL);
                    segment->SetType(5, MemoryType::LABEL);
                }

                // create a new segment.
                segment->wBegin = cassetteSegment->loadAddress;
                segment->wEnd = segment->wBegin + iOffset - 1;
                segment->bBinary = true;

                // forget this segment.
                firstSegment = false;
                iOffset = 0;
            }

            // skip first 3 bytes of the data (usually $55 $55 $FC)
            inputStream.Skip(3);

            bytesRemaining -= 4;
            header.length -= 4;

            // allocate a buffer to hold real data
            if (szBuf != nullptr) {
                delete[] szBuf;
            }

            szBuf = new byte[iOffset + header.length];

            // read data excluding last byte (probably a checksum byte)
            inputStream.Read(&szBuf[iOffset], header.length);

            iOffset += header.length;
            bytesRemaining -= header.length;

            // skip last byte of the data (probably a checksum byte)
            inputStream.Skip(1);

            // check if we have a new segment
            if (expectedSegmentCount == 0) {
                const CAS_SEGMENT* cassetteSegment = (CAS_SEGMENT*)szBuf;

                expectedSegmentCount = cassetteSegment->segmentCount;
                if (expectedSegmentCount == 0) {
                    expectedSegmentCount = 256;
                }
            }

            expectedSegmentCount--;
        }
    }

    // save previous segment
    if ((szBuf != nullptr) && (iOffset > 0)) {
        // check if segment is valid
        const CAS_SEGMENT* cassetteSegment = (CAS_SEGMENT*)szBuf;

        if ((cassetteSegment->segmentCount != 0) || (cassetteSegment->unused != 0) || (cassetteSegment->initAddress != 0) || (cassetteSegment->loadAddress != 0))
        {
            // allocate memory for data and for byte type.
            const auto& segment = segmentListInserter.InsertSegment();
            segment->CreateMemoryBlockWithSize(iOffset);
            const ByteArray buffer = ByteArray(szBuf, segment->GetSize());
            segment->SetData(0, buffer, 0, buffer.size());

            if (firstSegment == true) {
                segment->SetType(0, MemoryType::BYTE);
                segment->SetType(1, MemoryType::BYTE);
                segment->SetType(2, MemoryType::LABEL);
                segment->SetType(3, MemoryType::LABEL);
                segment->SetType(4, MemoryType::LABEL);
                segment->SetType(5, MemoryType::LABEL);
            }

            // create a new segment.
            segment->wBegin = cassetteSegment->loadAddress;
            segment->wEnd = segment->wBegin + iOffset - 1;
            segment->bBinary = true;
        }

        delete[] szBuf;
    }
}
