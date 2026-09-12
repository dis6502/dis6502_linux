#include "FileDialogs.h"
#include "ProfilesController.h"
#include "FileType.h"
#include "ProfileLogic.h"
#include "Window.h"
#include <memory>


extern std::unique_ptr<FileDialogs> g_FileDialogs;

ProfilesController::ProfilesController(ProfileLogic& profileLogic) {
    this->profileLogic = &profileLogic;
}

bool ProfilesController::Load(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, const Window& parentWindow) {
    auto result = ::g_FileDialogs->ChooseOpenFileName(parentWindow, L"", FileType::PROFILE_FILE);
    if (result.success) {
        return profileLogic->LoadAndSetDefaultProfile(profile, computerSystemTypeInfo, result.filePath);
    }
    return false;
}

void ProfilesController::Save(const Profile& profile, const Window& parentWindow) {

    auto result = ::g_FileDialogs->ChooseSaveFileName(parentWindow, L"", FileType::PROFILE_FILE);
    if (result.success) {

        profileLogic->Save(profile, result.filePath);
    }
}
