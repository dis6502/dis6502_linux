#include "ProfileLogic.h"

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "Byte.h"
#include "systems/ComputerSystemType.h"
#include "FileInputStream.h"
#include "FileIO.h"
#include "FileSystemLogic.h"
#include "FileType.h"
#include "Profile.h"
#include "Profile1X.h"
#include "XML.h"

extern std::unique_ptr<Application> g_Application;
extern std::unique_ptr<FileSystemLogic> g_FileSystemLogic;


void ProfileLogic::LoadDefaultProfile(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo) {
    wstring filePath;

    auto settingsSection = ::g_Application->GetSettingsSection(computerSystemTypeInfo.id);

    settingsSection->GetString(L"LastProfile", filePath);
    if (!filePath.empty()) {
        Load(profile, filePath);
    }
}

bool ProfileLogic::Load(Profile& profile, wstring_view filePath) {
    bool result = false;

    ::g_Application->SendInfoMessageWithID(IDS_LOG_OPEN_PROFILE_FILE, filePath);
    try {
        auto byteArray = FileIO::ReadByteArray(filePath);

        if (byteArray.empty()) {
            g_Application->SendFileErrorMessageWithID(IDS_FILE_IO_EX_OPENING_FILE_FOR_READ_ACCESS, filePath); // TODO: Make IOException with reasonable text
        }
        else {
            if (Profile1X::Load(profile, byteArray)) {
                result = true;
            }
            else if (XML::Load(profile, L"Profile", filePath) == XML::Error::XML_SUCCESS) {
                result = true;
            }
            else {
                ::g_Application->SendErrorMessageWithID(IDS_ERR_BAD_PROFILE); // TODO: Make IOException with reasonable text
            }
        }
    }
    catch (const IOException& ex) {
        g_Application->SendErrorMessageWithException(ex);
    }


    return result;
}

bool ProfileLogic::Load(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, const Window& parentWindow) {
    auto result = ::g_FileSystemLogic->GetOpenFileName(parentWindow, L"", FileType::PROFILE_FILE);

    if (result.success) {
        if (Load(profile, result.filePath)) {
            auto settingsSection = ::g_Application->GetSettingsSection(computerSystemTypeInfo.id);

            settingsSection->WriteString(L"LastProfile", result.filePath);

            return true;
        }
    }

    return false;
}

void ProfileLogic::Save(const Profile& profile, const Window& parentWindow) {

    auto result = ::g_FileSystemLogic->GetSaveFileName(parentWindow, L"", FileType::PROFILE_FILE);
    if (result.success) {
        ::g_Application->SendInfoMessageWithID(IDS_LOG_SAVE_PROFILE_FILE, result.filePath);

        const auto xmlResult = XML::Save(profile, L"Profile", result.filePath); // TODO Use IOException
        if (xmlResult != XML::Error::XML_SUCCESS) {
            // Error: Cannot write to file {0}. Error code {1} - {2}
            auto xmlResultText = String::ansi_to_wstring(XML::Document::ErrorIDToName(xmlResult));
            ::g_Application->SendErrorMessageWithID(IDS_FILE_IO_ERR_WRITING_FILE, result.filePath, std::to_wstring(xmlResult), xmlResultText);
        }
    }
}
