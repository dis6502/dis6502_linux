#pragma once

#include "../ComputerSystem.h"

#include "../../Memory.h"

class Atari800 final : public ComputerSystem {
public:
    Atari800(const ComputerSystemTypeInfo& computerSytemTypeInfo);

    bool IsBaseAddress(const Memory::address wAddr) const  override;
    bool IsVectorAddress(const Memory::address wAddr) const override;
    bool IsDisplayListVectorAddress(const Memory::address wAddr) const override;

    FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const override;
    void WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool writeHeader, OutputStream& outputStream) const override;

protected:
    void ReadCassetteFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;
    void ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;
    void ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;

private:
    static constexpr byte CAR_HEADER_SIZE = 16;
    static constexpr Memory::size SIZE_4K = 0x1000;
    static constexpr Memory::size SIZE_8K = 0x2000;
    static constexpr Memory::size SIZE_16K = 0x4000;

    static constexpr Memory::size SIZE_4K_CAR = SIZE_4K + CAR_HEADER_SIZE;
    static constexpr Memory::size SIZE_8K_CAR = SIZE_8K + CAR_HEADER_SIZE;
    static constexpr Memory::size SIZE_16K_CAR = SIZE_16K + CAR_HEADER_SIZE;
#
    bool FakeSegment(Segment& segment) const;
    bool LoadFixUps(Segment& Segment, InputStream& inputStream, FileIO::FILE_SIZE& bytesRemaining) const;
};
