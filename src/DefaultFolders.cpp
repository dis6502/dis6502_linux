#include "systems/ComputerSystemType.h"
#include "systems/ComputerSystemFactory.h"
#include "FolderType.h"

#include "DefaultFolders.h"
#include "PlatformCompat.h"

DefaultFolders::DefaultFolders(const ComputerSystemTypeInfo& computerSystemTypeInfo) :
    computerSystemTypeInfo(&computerSystemTypeInfo) {
	folders = std::make_unique<std::map<wstring, wstring>>();
}

gsl::not_null<const ComputerSystemTypeInfo*> DefaultFolders::GetComputerSystemTypeInfo() const {
    return computerSystemTypeInfo;
}

wstring_view DefaultFolders::GetFolderPath(const FolderType folderType) const {
	try {
		return folders->at(FolderTypeFactory::GetInfo(folderType).key);
	}
	catch (const std::out_of_range&) {
		return L"";
	}
}

void DefaultFolders::CopyFolderPath(const FolderType folderType, FileIO::FOLDER_PATH szFolderPath) const {
	const auto folderPath = GetFolderPath(folderType);
    wcsncpy_s(szFolderPath, FileIO::FOLDER_PATH_SIZE, folderPath.data(), folderPath.length());
}

void DefaultFolders::SetFolderPath(const FolderType folderType, wstring_view folderPath) {
	auto key = FolderTypeFactory::GetInfo(folderType).key;

	std::map<wstring, wstring>::iterator it = folders->find(key);
	if (it == folders->end()) {
		folders->insert(std::pair<wstring, wstring>(key, folderPath));
	}
	else {
		it->second = folderPath;
	}
}
