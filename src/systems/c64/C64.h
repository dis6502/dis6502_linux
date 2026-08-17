#pragma once

#pragma once

#include "ComputerSystem.h"

#include "Memory.h"

class C64 final : public ComputerSystem {
public:
    C64(const ComputerSystemTypeInfo& computerSystemTypeInfo);

    bool IsBaseAddress(const Memory::address wAddr) const  override;
    bool IsVectorAddress(const Memory::address wAddr) const override;

    FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const override;
    void WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool writeHeader, OutputStream& outputStream) const override;

protected:
    //void ReadCassetteFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;
    void ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;
    //void ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;

private:
    static constexpr Memory::size PRG_HEADER_SIZE = 2;
    static constexpr FileIO::FILE_SIZE PRG_MAX_SIZE = PRG_HEADER_SIZE + 0x10000;
};
