#pragma once

#include "systems/ComputerSystemType.h"
#include <list>
#include <memory>

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
