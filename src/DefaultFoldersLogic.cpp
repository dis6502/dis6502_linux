#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "systems/ComputerSystemType.h"
#include "DefaultFolders.h"
#include "FileIO.h"
#include "FolderType.h"

#include "DefaultFoldersLogic.h"

extern std::unique_ptr<Application> g_Application;

DefaultFoldersLogic::DefaultFoldersLogic() {
	folderTypeList = std::make_unique<std::list<FolderType>>();
	folderTypeList->emplace_back(FolderType::UNKNOWN_FILES);
	folderTypeList->emplace_back(FolderType::RAW_FILES);
	folderTypeList->emplace_back(FolderType::EXECUTABLE_FILES);
	folderTypeList->emplace_back(FolderType::ROM_IMAGE_FILES);
	folderTypeList->emplace_back(FolderType::CASSETTE_IMAGE_FILES);
	folderTypeList->emplace_back(FolderType::DISK_IMAGE_FILES);
	folderTypeList->emplace_back(FolderType::WORKSPACE_FILES);
	folderTypeList->emplace_back(FolderType::EQUATES_FILES);
	folderTypeList->emplace_back(FolderType::PROFILE_FILES);
	folderTypeList->emplace_back(FolderType::DISASSEMBLY_FILES);
}

std::unique_ptr<DefaultFolders> DefaultFoldersLogic::CreateDefaultFolders(const ComputerSystemTypeInfo& computerSystemTypeInfo) {

	auto defaultFolders = std::make_unique<DefaultFolders>(computerSystemTypeInfo);

	// Set default for all folder paths based on application module path.
	auto folderPath = ::g_Application->GetModuleFilePath(L"");

	defaultFolders->SetFolderPath(FolderType::RAW_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::EXECUTABLE_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::ROM_IMAGE_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::CASSETTE_IMAGE_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::DISK_IMAGE_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::WORKSPACE_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::EQUATES_FILES, folderPath);
	defaultFolders->SetFolderPath(FolderType::DISASSEMBLY_FILES, folderPath);

	folderPath = ::g_Application->GetModuleFilePath(L"profiles");
	defaultFolders->SetFolderPath(FolderType::PROFILE_FILES, folderPath);

	return std::move(defaultFolders);
}

void DefaultFoldersLogic::Load(DefaultFolders& defaultFolders) {

	const auto& info = defaultFolders.GetComputerSystemTypeInfo();
	const auto settings(::g_Application->GetSettingsSection(info->id));

	for (const auto& folderType : *folderTypeList) {
		wstring folderPath;
		settings->GetString(FolderTypeFactory::GetInfo(folderType).key, folderPath);
		if (!folderPath.empty()) {
			defaultFolders.SetFolderPath(folderType, folderPath);
		}
	}
}

void DefaultFoldersLogic::Save(const DefaultFolders& defaultFolders) {
	const auto& info = defaultFolders.GetComputerSystemTypeInfo();
	const auto settings = ::g_Application->GetSettingsSection(info->id);

	for (const auto& folderType : *folderTypeList) {
		settings->WriteString(FolderTypeFactory::GetInfo(folderType).key, defaultFolders.GetFolderPath(folderType));
	}
}
