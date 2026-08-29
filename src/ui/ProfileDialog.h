#pragma once

#include "Syntax.h"

#include "Dialog.h"
#include <vector>
#include "Encoding.h"

class ComputerSystemTypeInfo;
class Profile;
class ProfileLogic;


class ProfileDialog : public Dialog {
public:
    ProfileDialog(Window& parentWindow, ProfileLogic& profileLogic);

    bool Show(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

private:
    ProfileLogic* profileLogic;
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
