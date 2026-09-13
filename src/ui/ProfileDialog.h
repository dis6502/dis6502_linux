#pragma once


#include "Dialog.h"
#include "Encoding.h"
#include "Window.h"
#include <vector>

class ComputerSystemTypeInfo;
class Profile;
class ProfilesController;


class ProfileDialog : public Dialog {
public:
    ProfileDialog(Window& parentWindow, ProfilesController& profilesController);

    bool Show(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo);

protected:
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;
    bool OnCancel() override;

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
