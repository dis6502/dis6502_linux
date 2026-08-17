#pragma once

#include "ComputerSystem.h"

class Oric final : public ComputerSystem {
public:
	Oric(const ComputerSystemTypeInfo& computerSystemTypeInfo);

	bool IsBaseAddress(const Memory::address address) const override;
	bool IsVectorAddress(const Memory::address address) const override;

	FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const override;
	void ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, FileIO::FILE_SIZE fileSize) const override;
};
