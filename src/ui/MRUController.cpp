#include "FileType.h"
#include "MRUController.h"
#include "MRUList.h"
#include "MRUMenu.h"
#include "Resource.h"

MRUController::MRUController() :
    workspaceList(std::make_unique<MRUList>(L"RecentWorkspaces", MRU_MAX_ENTRIES)),
    fileList(std::make_unique<MRUList>(L"RecentFiles", MRU_MAX_ENTRIES)),

    workspaceMenu(std::make_unique<MRUMenu>(MRU_WORKSPACE_INDEX, std::vector<UINT>{IDM_MRU_WORKSPACE_1, IDM_MRU_WORKSPACE_2, IDM_MRU_WORKSPACE_3, IDM_MRU_WORKSPACE_4, IDM_MRU_WORKSPACE_5})),
    fileMenu(std::make_unique<MRUMenu>(MRU_FILE_INDEX, std::vector<UINT>{IDM_MRU_FILE_1, IDM_MRU_FILE_2, IDM_MRU_FILE_3, IDM_MRU_FILE_4, IDM_MRU_FILE_5})) {
}

void MRUController::FillMenu(HMENU hMenu) {

    workspaceMenu->FillMenu(hMenu, *workspaceList);
    fileMenu->FillMenu(hMenu, *fileList);
}

void MRUController::Load() {
    workspaceList->Load();
    fileList->Load();
}

void MRUController::Save() {
    workspaceList->Save();
    fileList->Save();
}

void MRUController::AddFile(wstring_view filePath, const FileType fileType) {
    if (filePath.empty()) {
        return;
    }

    if (fileType == FileType::WORKSPACE_FILE) {
        workspaceList->AddFile(filePath, fileType);
    }
    else {
        fileList->AddFile(filePath, fileType);
    }
}

wstring MRUController::GetLastFilePath(const FileType fileType) {
    if (fileType == FileType::WORKSPACE_FILE) {
        return workspaceList->GetLastFilePath(fileType);
    }
    else {
        return fileList->GetLastFilePath(fileType);
    }
}

const MRUEntry* MRUController::GetMRUEntry(HMENU hMenu, UINT uMenuID) const {
    const MRUEntry* entry = fileMenu->GetMRUEntry(hMenu, uMenuID);
    if (entry == nullptr) {
        entry = workspaceMenu->GetMRUEntry(hMenu, uMenuID);
    }

    return entry;
}
