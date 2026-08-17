#pragma once

#include "Syntax.h"

#include "systems/ComputerSystemType.h"
#include "FileIO.h"
#include <gsl/pointers>
#include <map>
#include <gsl/pointers>

enum class FolderType;

class DefaultFolders {
public:
	DefaultFolders(const ComputerSystemTypeInfo& computerSystemType);

    gsl::not_null<const ComputerSystemTypeInfo*>  GetComputerSystemTypeInfo() const;
	wstring_view GetFolderPath(const FolderType folderType) const;
	void CopyFolderPath(const FolderType folderType, FileIO::FOLDER_PATH szFolderPath) const;
	void SetFolderPath(const FolderType folderType, wstring_view folderPath);

private:
	friend class DefaultFoldersLogic;
	friend class DefaultFoldersDialog;

    gsl::not_null<const ComputerSystemTypeInfo*> computerSystemTypeInfo;
	std::unique_ptr<std::map<wstring, wstring>> folders;
};
