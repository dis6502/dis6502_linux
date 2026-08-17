#pragma once

#include "ComputerSystem.h"

class Atari5200 final : public ComputerSystem {
public:
	Atari5200(const ComputerSystemTypeInfo& computerSystemTypeInfo);

	bool IsBaseAddress(const Memory::address wAddr) const override;
	bool IsVectorAddress(const Memory::address wAddr) const override;
	bool IsDisplayListVectorAddress(const Memory::address wAddr) const override;

	FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const override;
	void ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const override;
};
