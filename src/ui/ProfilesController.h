#pragma once

#include "ProfileLogic.h"

class Profile;
class ComputerSystemTypeInfo;
class Window;

class ProfilesController {
public:
    ProfilesController(ProfileLogic& profileLogic);

    bool Load(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, const Window& parentWindow);
    void Save(const Profile& profile, const Window& parentWindow);


private:
    ProfileLogic* profileLogic;
};
