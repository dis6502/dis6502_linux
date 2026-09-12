#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "CommonIO.h"
#include "FileIO.h" 
#include "Profile.h"
#include "Profile1X.h"
#include "ProfileLogic.h"
#include "Resource.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "systems/ComputerSystemType.h"
#include "Text.h"
#include "XML.h"
#include <string>

extern Application* g_Application;


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
            throw IOException(FileIO::FormatError(IDS_FILE_IO_EX_OPENING_FILE_FOR_READ_ACCESS, filePath));
        }
        else {
            if (Profile1X::Load(profile, byteArray)) {
                result = true;
            }
            else if (XML::Load(profile, L"Profile", filePath) == XML::Error::XML_SUCCESS) {
                result = true;
            }
            else {
                throw IOException(Text::Format(IDS_ERR_BAD_PROFILE, filePath)); // TODO: Have own text ID
            }
        }
    }
    catch (const IOException& ex) {
        g_Application->SendErrorMessageWithException(ex);
    }

    return result;
}

bool ProfileLogic::LoadAndSetDefaultProfile(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, wstring_view filePath) {

    if (Load(profile, filePath)) {
        auto settingsSection = ::g_Application->GetSettingsSection(computerSystemTypeInfo.id);

        settingsSection->WriteString(L"LastProfile", filePath);

        return true;
    };
    return false;
}

void ProfileLogic::Save(const Profile& profile, wstring_view filePath) {
    ::g_Application->SendInfoMessageWithID(IDS_LOG_SAVE_PROFILE_FILE, filePath);

    const auto xmlResult = XML::Save(profile, L"Profile", filePath); // TODO Use IOException
    if (xmlResult != XML::Error::XML_SUCCESS) {
        // Error: Cannot write to file {0}. Error code {1} - {2}
        auto xmlResultText = String::ansi_to_wstring(XML::Document::ErrorIDToName(xmlResult));
        ::g_Application->SendErrorMessageWithID(IDS_FILE_IO_ERR_WRITING_FILE, filePath, std::to_wstring(xmlResult), xmlResultText);
    }
}
