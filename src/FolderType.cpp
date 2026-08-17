#include "FolderType.h"

#include <map>

FolderTypeInfo FolderTypeFactory::GetInfo(wstring_view key) {
	static std::map<wstring_view, FolderType> typeLookup = {
		{ L"RAW_FILES", FolderType::RAW_FILES },
		{ L"EXECUTABLE_FILES", FolderType::EXECUTABLE_FILES },
		{ L"ROM_IMAGE_FILES", FolderType::ROM_IMAGE_FILES },
		{ L"CASSETTE_IMAGE_FILES", FolderType::CASSETTE_IMAGE_FILES },
		{ L"DISK_IMAGE_FILES", FolderType::DISK_IMAGE_FILES },
		{ L"WORKSPACE_FILES", FolderType::WORKSPACE_FILES },
		{ L"EQUATES_FILES", FolderType::EQUATES_FILES },
		{ L"PROFILE_FILES", FolderType::PROFILE_FILES },
		{ L"DISASSEMBLY_FILES", FolderType::DISASSEMBLY_FILES }
	};

	FolderType folderType = FolderType::UNKNOWN_FILES;
	try {
		folderType = typeLookup.at(key);
	}
	catch (const std::out_of_range&) {}

	return GetInfo(folderType);
}

FolderTypeInfo FolderTypeFactory::GetInfo(const FolderType type) {
	if (type == FolderType::RAW_FILES)
		return FolderTypeInfo(FolderType::RAW_FILES, L"RAW_FILES", L"Raw Files");
	else if (type == FolderType::EXECUTABLE_FILES)
		return FolderTypeInfo(FolderType::EXECUTABLE_FILES, L"EXECUTABLE_FILES", L"Executable Files");
	else if (type == FolderType::ROM_IMAGE_FILES)
		return FolderTypeInfo(FolderType::ROM_IMAGE_FILES, L"ROM_IMAGE_FILES", L"ROM Image Files");
	else if (type == FolderType::CASSETTE_IMAGE_FILES)
		return FolderTypeInfo(FolderType::CASSETTE_IMAGE_FILES, L"CASSETTE_IMAGE_FILES", L"Cassette Image Files");
	else if (type == FolderType::DISK_IMAGE_FILES)
		return FolderTypeInfo(FolderType::DISK_IMAGE_FILES, L"DISK_IMAGE_FILES", L"Disk Image Files");
	else if (type == FolderType::WORKSPACE_FILES)
		return FolderTypeInfo(FolderType::WORKSPACE_FILES, L"WORKSPACE_FILES", L"Workspace Files");
	else if (type == FolderType::EQUATES_FILES)
		return FolderTypeInfo(FolderType::EQUATES_FILES, L"EQUATES_FILES", L"Equates Files");
	else if (type == FolderType::PROFILE_FILES)
		return FolderTypeInfo(FolderType::PROFILE_FILES, L"PROFILE_FILES", L"Profile Files");
	else if (type == FolderType::DISASSEMBLY_FILES)
		return FolderTypeInfo(FolderType::DISASSEMBLY_FILES, L"DISASSEMBLY_FILES", L"Disassembly Files");

	return FolderTypeInfo(FolderType::UNKNOWN_FILES, L"UNKNOWN", L"Unknown");
}
