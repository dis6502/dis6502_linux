#pragma once

#include "Syntax.h"


enum class FolderType {
	UNKNOWN_FILES,
	RAW_FILES,
	EXECUTABLE_FILES,
	ROM_IMAGE_FILES,
	CASSETTE_IMAGE_FILES,
	DISK_IMAGE_FILES,
	WORKSPACE_FILES,
	EQUATES_FILES,
	PROFILE_FILES,
	DISASSEMBLY_FILES,
};

struct FolderTypeInfo {
	const FolderType type;
	const wstring key;
	const wstring text;

	FolderTypeInfo(const FolderType type, const wstring_view key, const wstring_view text) :
		type{ type }, key{ key }, text{ text }
	{
	}
};

class FolderTypeFactory {
public:
	static FolderTypeInfo GetInfo(const wstring_view key);
	static FolderTypeInfo GetInfo(const FolderType type);
};
