#pragma once

#include "ComputerSystem.h"


namespace systems {

	class Unknown final : public ComputerSystem {
	public:
		Unknown(const ComputerSystemTypeInfo& computerSystemTypeInfo);

		bool IsBaseAddress(const Memory::address address) const  override;
		bool IsVectorAddress(const Memory::address address) const override;
		bool IsDisplayListVectorAddress(const Memory::address address) const override;

		FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const override;
	};

}