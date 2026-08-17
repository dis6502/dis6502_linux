#include "Profile.h"

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "Strings.h"

#include "Profile1X.h"

constexpr size_t PROFILE1X_MAGIC_LENGTH = 12;
constexpr auto PROFILE10_MAGIC = "DIS6502PRF10";
constexpr auto PROFILE13_MAGIC = "DIS6502PRF13";
constexpr auto PROFILE16_MAGIC = "DIS6502PRF16";
constexpr auto PROFILE17_MAGIC = "DIS6502PRF17";

extern std::unique_ptr<Application> g_Application;


bool Profile1X::Load(Profile& profile, const ByteSequence& buffer) {
    bool result = true;

    PROFILE17 profileStruct = {};
    const auto magic = buffer.stringAt(0, PROFILE1X_MAGIC_LENGTH);
    if (magic == PROFILE10_MAGIC || magic == PROFILE13_MAGIC || magic == PROFILE16_MAGIC || magic == PROFILE17_MAGIC) {
        byte* outBuffer = (byte*)&profileStruct;
        for (size_t i = 0; i < buffer.size(); ++i) {
            outBuffer[i] = buffer.at(i + 12);
        }
    }
    else {
        result = false;
    }

    /*
    ** If reading was successful, map the old structure to the new class.
    ** For compatibility with previous storage system, some settings are defaulted from the default config section.
    */
    if (result) {
        auto settingsSections = ::g_Application->GetSettingsSection(L"DefaultConfig");

        // Copy attributes. Order is alphabethical for easier tracking.
        profile.alignInstructions = profileStruct.alignInstructions;
        profile.directiveBYTEOnlyNumbersAllowed = profileStruct.directiveBYTEOnlyNumbersAllowed;
        profile.directiveDSAllowed = profileStruct.directiveDSAllowed;
        profile.directiveENDNeedsFilename = profileStruct.directiveENDNeedsFilename;
        profile.directiveINCLUDEAllEquatesInOneIncludeFile = false; // Did not exist
        profile.directiveINCLUDEAllIncludesInMainFile = profileStruct.directiveINCLUDEAllIncludesInMainFile;
        profile.directiveINCLUDEAllowed = profileStruct.directiveINCLUDEAllowed;
        profile.directiveSBYTEAllowed = profileStruct.directiveSBYTEAllowed;
        profile.directiveWORDAllowed = profileStruct.directiveWORDAllowed;
        profile.omitUnreferencedSystemLabels = true; // Did not exist
        profile.showAInAccumulatorMode = profileStruct.showAInAccumulatorMode;
        profile.showBRKAsByte0 = profileStruct.showBRKAsByte0;
        profile.showColonAfterLabel = profileStruct.showColonAfterLabel;
        profile.showLowerCaseInstructions = profileStruct.showLowerCaseInstructions;
        profile.showNonASCIIChararactersAsBytes = true; // Did not exist
        unsigned int value;
        settingsSections->GetUnsignedInt(L"DisplayOpcodes", value);
        profile.showOpcodeAsComment = static_cast<bool>(value);
        profile.showZPAbsoluteAsByte = profileStruct.showZPAbsoluteAsByte;
        profile.useHexNotation = profileStruct.useHexNotation;
        profile.useIllegalOpcodes = profileStruct.useIllegalOpcodes;
        profile.useLineNumbers = profileStruct.useLineNumbers;

        profile.commentPrefix = String::FromCharArray(profileStruct.commentPrefix, sizeof(profileStruct.commentPrefix));
        profile.directiveBYTE = String::FromCharArray(profileStruct.directiveBYTE, sizeof(profileStruct.directiveBYTE));
        profile.directiveBYTESeparator = String::FromCharArray(profileStruct.directiveBYTESeparator, sizeof(profileStruct.directiveBYTESeparator));
        profile.directiveDS = String::FromCharArray(profileStruct.directiveDS, sizeof(profileStruct.directiveDS));
        profile.directiveENDHead = String::FromCharArray(profileStruct.directiveENDHead, sizeof(profileStruct.directiveENDHead));
        profile.directiveENDTail = String::FromCharArray(profileStruct.directiveENDTail, sizeof(profileStruct.directiveENDTail));
        profile.directiveEQU = String::FromCharArray(profileStruct.directiveEQU, sizeof(profileStruct.directiveEQU));
        profile.directiveHIGHHead = String::FromCharArray(profileStruct.directiveHIGHHead, sizeof(profileStruct.directiveHIGHHead));
        profile.directiveHIGHTail = String::FromCharArray(profileStruct.directiveHIGHTail, sizeof(profileStruct.directiveHIGHTail));
        profile.directiveINCLUDEHead = String::FromCharArray(profileStruct.directiveINCLUDEHead, sizeof(profileStruct.directiveINCLUDEHead));
        profile.directiveINCLUDETail = String::FromCharArray(profileStruct.directiveINCLUDETail, sizeof(profileStruct.directiveINCLUDETail));
        profile.directiveLOWHead = String::FromCharArray(profileStruct.directiveLOWHead, sizeof(profileStruct.directiveLOWHead));
        profile.directiveLOWTail = String::FromCharArray(profileStruct.directiveLOWTail, sizeof(profileStruct.directiveLOWTail));
        profile.directiveORG = String::FromCharArray(profileStruct.directiveORG, sizeof(profileStruct.directiveORG));
        profile.directiveSBYTE = String::FromCharArray(profileStruct.directiveSBYTE, sizeof(profileStruct.directiveSBYTE));
        profile.directiveWORD = String::FromCharArray(profileStruct.directiveWORD, sizeof(profileStruct.directiveWORD));

        profile.directiveBYTENumberOfBytesPerLine = value;
        settingsSections->GetUnsignedInt(L"NbCharPerString", value, 40);
        profile.directiveBYTENumberOfCharactersPerString = value;
        profile.directiveINCLUDEMaximumNumberOfLinesPerFile = profileStruct.directiveINCLUDEMaximumNumberOfLinesPerFile;
        settingsSections->GetUnsignedInt(L"NbWordsPerLine", value, 8);
        profile.directiveWORDNumberOfWordsPerLine = value;

        delete settingsSections;
    }

    return result;
}
