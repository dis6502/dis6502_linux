#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include "Encoding.h"
#include <vector>

class ComputerSystemTypeInfo;
class Profile;
class ProfilesController;


class ProfileDialog : public Dialog {
public:
    ProfileDialog(Window& parentWindow, ProfilesController& profilesController);

    bool Show(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo);

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
    ProfilesController* profilesController;
    Profile* profile = nullptr;
    const ComputerSystemTypeInfo* computerSystemTypeInfo = nullptr;
    std::vector<Encoding> outputEncodings = { Encoding::ASCII, Encoding::ATASCII, Encoding::UTF8 };

    /*
    ** Fill all dialog box fields with values from instance.
    */
    void SetDialogValues(const Profile& profile);

    /*
    ** Fill instance with values from dialog box fields.
    */
    void GetDialogValues(Profile& profile);
};
