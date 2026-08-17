#include "Oric.h"

#include "../../FileType.h"
#include "../../InputStream.h"
#include "../../OutputStream.h"
#include "../../Segment.h"
#include "../../SegmentListInserter.h"
#include "../../Workspace.h"
#include <set>


/**

The Oric ".tap" file format description according to https://forum.defence-force.org/viewtopic.php?f=19&t=201

$16
$16
$16 (Minimum of three $16's)

$24 (Synchronisation byte)

$00
$00

$00 (Basic) or $80 (Assembler)

$00 (No Autostart) or $80 (Basic Autostart) or $C7 (Assembly Autostart)

High Byte of the End address
Low Byte of the End Address

High Byte of the Start Address
Low Byte of the Start Address

$0

Filename(Up to 16 characters)

$0

Program Data

 */
Oric::Oric(const ComputerSystemTypeInfo& computerSystemTypeInfo) :ComputerSystem(computerSystemTypeInfo) {
    returnCharacter = 0x0d;

    supportedFileTypes = std::vector<FileType>{
        FileType::RAW_FILE,
        FileType::EXECUTABLE_FILE
    };

}

bool Oric::IsBaseAddress(const Memory::address address) const {
    static std::set<Memory::address> addresses = {
        0x0238, /*XVDU*/
        0x023B, /*XGETKY*/
        0x023E, /*XPRTCH*/
        0x0241, /*XSTOUT*/
        0x0244, /*INTFS*/
        0x02E0, /*BAPARM*/
        0x02FC  /*ASMADD*/
    };

    if (addresses.contains(address)) {
        return true;
    }

    return ((address & 0xFF00) == 0); // TODO: Are these really base addresses on Oric?
}

bool Oric::IsVectorAddress(const Memory::address address) const {
    return false;
}

FileType Oric::GuessFileType(const FileIO::FILE_SIZE lFileSize, ByteSequence& content) const {
    // TODO:
    // See Orix header format https://orix.oric.org/orix-header/ 
    // See tape header format https://forum.defence-force.org/viewtopic.php?t=201

    static constexpr unsigned short headerValue = static_cast<unsigned short>(FileHeader::ORIC_BINARY);

    if ((content.at(0) == (headerValue & 0xff)) && (content.at(1) == ((headerValue >> 8) & 0xff))) {
        return FileType::EXECUTABLE_FILE;
    }

    return FileType::UNKNOWN_FILE;
}

void Oric::ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, FileIO::FILE_SIZE fileSize) const {
    FileHeader wHeader;
    char szBuf[8];

    // Check that the file begins with 2 0xFF.
    inputStream.Read(&wHeader, sizeof(wHeader));

    if (wHeader != FileHeader::ORIC_BINARY) {
        throw IOException(L"Unsupported file header " + to_wstring(wHeader));
    }

    auto bytesRemaining = fileSize - sizeof(wHeader);

    // read all segments
    while (bytesRemaining > 0) {
        char cAsm = 0;

        // read bytes until we find an end of synchronisation mark.
        static constexpr byte END_OF_SYNC = 0x24;

        do {
            inputStream.Read(&cAsm, sizeof(cAsm));

            bytesRemaining--;
        } while (cAsm != END_OF_SYNC);

        // skip 2 bytes then read program type (Basic or 6502).
        inputStream.Read(szBuf, 2);

        inputStream.Read(&cAsm, sizeof(cAsm));

        inputStream.Read(szBuf, 1);

        bytesRemaining -= 4;

        // read segment start and end address.
        Memory::address wEnd = 0;
        inputStream.Read(&wEnd, sizeof(wEnd));
        wEnd = ((wEnd << 8) & 0xFF00) + ((wEnd >> 8) & 0x00FF);  // Swap bytes

        Memory::address wBegin = 0;
        inputStream.Read(&wBegin, sizeof(wBegin));
        wBegin = ((wBegin << 8) & 0xFF00) + ((wBegin >> 8) & 0x00FF); // Swap bytes

        if (wEnd < wBegin) {
            throw IOException(L"Segement end address is lower than segment start address");
        }

        bytesRemaining -= sizeof(wBegin);
        bytesRemaining -= sizeof(wEnd);

        // read name of program/data.
        inputStream.Read(szBuf, 1);

        bytesRemaining--;
        const auto& lpSegment = segmentListInserter.InsertSegment();

        int nNameIndex = 0;
        for (; nNameIndex < 16; nNameIndex++) {
            char c = 0;

            inputStream.Read(&c, sizeof(c));

            bytesRemaining--;

            if (c == 0) {
                break;
            }
            lpSegment->szTitle.push_back(c);
        }

        lpSegment->szTitle.at(nNameIndex) = 0;

        // check data size.
        WORD wSize = wEnd - wBegin + 1;
        if (wSize > bytesRemaining) {
            wSize = (WORD)bytesRemaining;
        }

        // save segment type
        lpSegment->bBinary = (bool)cAsm;

        // read segment data.
        lpSegment->wBegin = wBegin;
        lpSegment->wEnd = wEnd;
        lpSegment->CreateMemoryBlockFromFile(wSize, inputStream);
        bytesRemaining -= wSize;
    }
}
