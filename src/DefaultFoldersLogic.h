#pragma once

#include <list>

#include "Syntax.h"
#include "systems/ComputerSystemType.h"

class ApplicationSettingsSection;
class DefaultFolders;
enum class FolderType;


class DefaultFoldersLogic {
public:
	DefaultFoldersLogic();

	std::unique_ptr<DefaultFolders> CreateDefaultFolders(const ComputerSystemTypeInfo& computerSystemTypeInfo);
	
	void Load(DefaultFolders& defaultFolders);
	void Save(const DefaultFolders& defaultFolders);

private:
	std::unique_ptr<std::list<FolderType>> folderTypeList;
};
