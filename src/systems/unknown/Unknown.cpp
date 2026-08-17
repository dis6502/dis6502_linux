#include "Unknown.h"

#include "../../FileType.h"


namespace systems { // TODO: Use namespace systems also for other systems

	Unknown::Unknown(const ComputerSystemTypeInfo& computerSystemTypeInfo): ComputerSystem(computerSystemTypeInfo) {
		returnCharacter = 0x0a;

		supportedFileTypes = std::vector <FileType>{
			FileType::RAW_FILE
		};
	}

	bool Unknown::IsBaseAddress(const Memory::address address) const {
		return false;
	}

	bool Unknown::IsVectorAddress(const Memory::address address) const {
		return false;
	}

	bool Unknown::IsDisplayListVectorAddress(const Memory::address address) const {
		return false;
	}

	FileType Unknown::GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const {
		return FileType::UNKNOWN_FILE;
	}


}