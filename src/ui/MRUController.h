#pragma once

#include "Syntax.h"

enum class FileType;
class MRUEntry;

#include "MRUMenu.h" // Because of smart pointer, forward declartion not sufficient
#include "MRUList.h" // Because of smart pointer, forward declartion not sufficient

#include "Window.h"

class MRUController {
public:

	MRUController();

	void AddFile(wstring_view filePath, FileType fileType);
	wstring GetLastFilePath(FileType fileType);
	void FillMenu(HMENU hMenu);
	const MRUEntry* GetMRUEntry(HMENU hMenu, UINT uMenuID) const;

	void Load();
	void Save();

private:
	static constexpr int MRU_MAX_ENTRIES = 5;
	static constexpr int MRU_WORKSPACE_INDEX = 11;
	static constexpr int MRU_FILE_INDEX = 12;

	std::unique_ptr<MRUList> workspaceList;
	std::unique_ptr<MRUList> fileList;

	std::unique_ptr<MRUMenu> workspaceMenu;
	std::unique_ptr<MRUMenu> fileMenu;
};
