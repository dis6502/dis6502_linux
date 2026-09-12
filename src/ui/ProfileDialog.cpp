#include "Application.h"
#include "CheckBox.h"
#include "ComboBox.h"
#include "Dialog.h"
#include "EditControl.h"
#include "Encoding.h"
#include "Profile.h"
#include "ProfileDialog.h" 
#include "ProfilesController.h" 
#include "Text.h"
#include "systems/ComputerSystemType.h"
#include "systems/ComputerSystemType.h"

ProfileDialog::ProfileDialog(Window& parentWindow, ProfilesController& profilesController) : Dialog(parentWindow, L"PROFILEBOX") {
    this->profilesController = &profilesController;
}

bool ProfileDialog::Show(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo) {
    this->profile = &profile;
    this->computerSystemTypeInfo = &computerSystemTypeInfo;

    return ShowDialogBox();
}

void ProfileDialog::SetDialogValues(const Profile& profile) {
    // Limit edit size.
    GetEditControl(IDC_WORDPERLINE).SetTextSize(2);
    GetEditControl(IDC_BYTEPERLINE).SetTextSize(2);
    GetEditControl(IDC_CHARPERLINE).SetTextSize(2);

    GetComboBox(IDC_OUTPUT_ENCODING).Clear();
    for (auto outputEncoding : outputEncodings) {
        auto info = EncodingFactory::GetInfo(outputEncoding);
        GetComboBox(IDC_OUTPUT_ENCODING).AddString(info.text, (void*)info.encoding);
    }

    // Set dialog fields values.
    GetEditControl(IDC_COMMENT).SetText(profile.commentPrefix);

    GetEditControl(IDC_HEXNOTATION).SetEnabled(profile.useHexNotation);
    GetEditControl(IDC_HEXNOTATION).SetText(profile.hexNotationPrefix);

    GetCheckBox(IDC_ILLEGALINSTRUCTION).SetChecked(profile.useIllegalOpcodes);
    GetCheckBox(IDC_USEHEX).SetChecked(profile.useHexNotation);
    GetCheckBox(IDC_ALIGNINSTRUCTION).SetChecked(profile.alignInstructions);
    GetCheckBox(IDC_LINENUMBERING).SetChecked(profile.useLineNumbers);

    GetCheckBox(IDC_SHOWLOWERCASE).SetChecked(profile.showLowerCaseInstructions);
    GetCheckBox(IDC_SHOWIMPLICITE).SetChecked(profile.showAInAccumulatorMode);
    GetCheckBox(IDC_SHOWCOLON).SetChecked(profile.showColonAfterLabel);
    GetCheckBox(IDC_DISPLAYOPCODES).SetChecked(profile.showOpcodeAsComment);

    GetCheckBox(IDC_SHOWBYTE0).SetChecked(profile.showBRKAsByte0);
    GetCheckBox(IDC_SHOWZPASBYTE).SetChecked(profile.showZPAbsoluteAsByte);
    GetEditControl(IDC_FORCEABSOLUTE).SetEnabled(!profile.showZPAbsoluteAsByte);
    GetEditControl(IDC_FORCEABSOLUTE).SetText(profile.directiveForceAbsolute);
    GetCheckBox(IDC_NONASCIIASBYTES).SetChecked(profile.showNonASCIIChararactersAsBytes);

    GetEditControl(IDC_BYTEPERLINE).SetNumber(profile.directiveBYTENumberOfBytesPerLine);
    GetEditControl(IDC_WORDPERLINE).SetNumber(profile.directiveWORDNumberOfWordsPerLine);
    GetEditControl(IDC_CHARPERLINE).SetNumber(profile.directiveBYTENumberOfCharactersPerString);
    GetEditControl(IDC_QUOTEFORASCIISTRINGS).SetText(profile.quoteForASCIIStrings);

    GetEditControl(IDC_ORGSYNTAX).SetText(profile.directiveORG);
    GetEditControl(IDC_EQUSYNTAX).SetText(profile.directiveEQU);
    GetEditControl(IDC_ENDSYNTAX).SetText(profile.directiveENDHead);
    GetCheckBox(IDC_ENDFILENAME).SetChecked(profile.directiveENDNeedsFilename);
    GetEditControl(IDC_ENDTAIL).SetText(profile.directiveENDTail);
    GetEditControl(IDC_LOWHEADSYNTAX).SetText(profile.directiveLOWHead);
    GetEditControl(IDC_LOWTAILSYNTAX).SetText(profile.directiveLOWTail);
    GetEditControl(IDC_HIGHHEADSYNTAX).SetText(profile.directiveHIGHHead);
    GetEditControl(IDC_HIGHTAILSYNTAX).SetText(profile.directiveHIGHTail);

    // Section "Directives".
    GetCheckBox(IDC_NUMONLYINBYTE).SetChecked(profile.directiveBYTEOnlyNumbersAllowed);
    GetEditControl(IDC_BYTESYNTAX).SetText(profile.directiveBYTE);
    GetEditControl(IDC_BYTESEPARATOR).SetText(profile.directiveBYTESeparator);
    GetCheckBox(IDC_SBYTEALLOWED).SetChecked(profile.directiveSBYTEAllowed);
    GetEditControl(IDC_SBYTESYNTAX).SetEnabled(profile.directiveSBYTEAllowed);
    GetEditControl(IDC_SBYTESYNTAX).SetText(profile.directiveSBYTE);
    GetCheckBox(IDC_WORDALLOWED).SetChecked(profile.directiveWORDAllowed);
    GetEditControl(IDC_WORDSYNTAX).SetEnabled(profile.directiveWORDAllowed);
    GetEditControl(IDC_WORDSYNTAX).SetText(profile.directiveWORD);
    GetCheckBox(IDC_DSALLOWED).SetChecked(profile.directiveDSAllowed);
    GetEditControl(IDC_DSSYNTAX).SetEnabled(profile.directiveDSAllowed);
    GetEditControl(IDC_DSSYNTAX).SetText(profile.directiveDS);

    // Section "Disassembly Listing".
    GetComboBox(IDC_OUTPUT_ENCODING).SetSelectedIndex(EncodingFactory::GetIndex(profile.outputEncoding, outputEncodings, 0));
    GetCheckBox(IDC_REMOVEUNUSEDLABELS).SetChecked(profile.omitUnreferencedSystemLabels);
    const bool bIncludeAllowed = profile.directiveINCLUDEAllowed;
    GetCheckBox(IDC_INCLUDEALLOWED).SetChecked(bIncludeAllowed);
    GetEditControl(IDC_INCLUDEHEAD).SetEnabled(bIncludeAllowed);
    GetEditControl(IDC_INCLUDEHEAD).SetText(profile.directiveINCLUDEHead);
    GetEditControl(IDC_INCLUDETAIL).SetEnabled(bIncludeAllowed);
    GetEditControl(IDC_INCLUDETAIL).SetText(profile.directiveINCLUDETail);
    GetCheckBox(IDC_RADIOINCLUDE_ONE_FILE).SetEnabled(bIncludeAllowed);
    GetCheckBox(IDC_RADIOINCLUDE_ALL_FILES).SetEnabled(bIncludeAllowed);
    GetCheckBox(IDC_RADIOINCLUDE_NEXT_FILE).SetEnabled(bIncludeAllowed);
    const ITEM_ID checkedButtonItemID = (profile.directiveINCLUDEAllEquatesInOneIncludeFile ? IDC_RADIOINCLUDE_ONE_FILE : (profile.directiveINCLUDEAllIncludesInMainFile ? IDC_RADIOINCLUDE_ALL_FILES : IDC_RADIOINCLUDE_NEXT_FILE));
    CheckRadioButton(hDlg, IDC_RADIOINCLUDE_ALL_FILES, IDC_RADIOINCLUDE_ONE_FILE, checkedButtonItemID);
    GetEditControl(IDC_MAXINCLUDELINES).SetEnabled(bIncludeAllowed && !profile.directiveINCLUDEAllEquatesInOneIncludeFile);
    GetEditControl(IDC_MAXINCLUDELINES).SetNumber(profile.directiveINCLUDEMaximumNumberOfLinesPerFile);
}

void ProfileDialog::GetDialogValues(Profile& profile) {
    // Get dialog field values.
    profile.commentPrefix = GetEditControl(IDC_COMMENT).GetText();
    profile.hexNotationPrefix = GetEditControl(IDC_HEXNOTATION).GetText();;
    profile.useIllegalOpcodes = GetCheckBox(IDC_ILLEGALINSTRUCTION).IsChecked();
    profile.useHexNotation = GetCheckBox(IDC_USEHEX).IsChecked();
    profile.alignInstructions = GetCheckBox(IDC_ALIGNINSTRUCTION).IsChecked();
    profile.useLineNumbers = GetCheckBox(IDC_LINENUMBERING).IsChecked();

    profile.showLowerCaseInstructions = GetCheckBox(IDC_SHOWLOWERCASE).IsChecked();
    profile.showAInAccumulatorMode = GetCheckBox(IDC_SHOWIMPLICITE).IsChecked();
    profile.showColonAfterLabel = GetCheckBox(IDC_SHOWCOLON).IsChecked();
    profile.showOpcodeAsComment = GetCheckBox(IDC_DISPLAYOPCODES).IsChecked();

    profile.showBRKAsByte0 = GetCheckBox(IDC_SHOWBYTE0).IsChecked();
    profile.showZPAbsoluteAsByte = GetCheckBox(IDC_SHOWZPASBYTE).IsChecked();
    profile.directiveForceAbsolute = GetEditControl(IDC_FORCEABSOLUTE).GetText();
    profile.showNonASCIIChararactersAsBytes = GetCheckBox(IDC_NONASCIIASBYTES).IsChecked();

    profile.directiveBYTENumberOfBytesPerLine = GetEditControl(IDC_BYTEPERLINE).GetNumber();
    profile.directiveWORDNumberOfWordsPerLine = GetEditControl(IDC_WORDPERLINE).GetNumber();
    profile.directiveBYTENumberOfCharactersPerString = GetEditControl(IDC_CHARPERLINE).GetNumber();
    profile.quoteForASCIIStrings = GetEditControl(IDC_QUOTEFORASCIISTRINGS).GetText();

    // Section "Directives"
    profile.directiveORG = GetEditControl(IDC_ORGSYNTAX).GetText();
    profile.directiveENDHead = GetEditControl(IDC_ENDSYNTAX).GetText();
    profile.directiveENDNeedsFilename = GetCheckBox(IDC_ENDFILENAME).IsChecked();
    profile.directiveENDTail = GetEditControl(IDC_ENDTAIL).GetText();
    profile.directiveEQU = GetEditControl(IDC_EQUSYNTAX).GetText();
    profile.directiveLOWHead = GetEditControl(IDC_LOWHEADSYNTAX).GetText();
    profile.directiveLOWTail = GetEditControl(IDC_LOWTAILSYNTAX).GetText();
    profile.directiveHIGHHead = GetEditControl(IDC_HIGHHEADSYNTAX).GetText();
    profile.directiveHIGHTail = GetEditControl(IDC_HIGHTAILSYNTAX).GetText();

    profile.directiveBYTEOnlyNumbersAllowed = GetCheckBox(IDC_NUMONLYINBYTE).IsChecked();
    profile.directiveBYTE = GetEditControl(IDC_BYTESYNTAX).GetText();
    profile.directiveBYTESeparator = GetEditControl(IDC_BYTESEPARATOR).GetText();
    profile.directiveSBYTEAllowed = GetCheckBox(IDC_SBYTEALLOWED).IsChecked();
    profile.directiveSBYTE = GetEditControl(IDC_SBYTESYNTAX).GetText();
    profile.directiveWORDAllowed = GetCheckBox(IDC_WORDALLOWED).IsChecked();
    profile.directiveWORD = GetEditControl(IDC_WORDSYNTAX).GetText();
    profile.directiveDSAllowed = GetCheckBox(IDC_DSALLOWED).IsChecked();
    profile.directiveDS = GetEditControl(IDC_DSSYNTAX).GetText();

    // Section "Disassembly Listing".
    profile.outputEncoding = outputEncodings.at(GetComboBox(IDC_OUTPUT_ENCODING).GetSelectedIndex());
    profile.omitUnreferencedSystemLabels = GetCheckBox(IDC_REMOVEUNUSEDLABELS).IsChecked();

    profile.directiveINCLUDEAllowed = GetCheckBox(IDC_INCLUDEALLOWED).IsChecked();
    profile.directiveINCLUDEHead = GetEditControl(IDC_INCLUDEHEAD).GetText();
    profile.directiveINCLUDETail = GetEditControl(IDC_INCLUDETAIL).GetText();
    profile.directiveINCLUDEAllEquatesInOneIncludeFile = GetCheckBox(IDC_RADIOINCLUDE_ONE_FILE).IsChecked();
    profile.directiveINCLUDEAllIncludesInMainFile = GetCheckBox(IDC_RADIOINCLUDE_ALL_FILES).IsChecked();
    profile.directiveINCLUDEMaximumNumberOfLinesPerFile = GetEditControl(IDC_MAXINCLUDELINES).GetNumber();
}

bool ProfileDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {

    switch (message) {
    case WM_INITDIALOG:
        SetDialogValues(*profile);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            GetDialogValues(*profile);
            EndDialog(hDlg, true);
            return true;
        }

        case IDCANCEL: {
            EndDialog(hDlg, false);
            return true;
        }

        case ID_LOAD_PROFILE: {
            Profile tempProfile;

            if (profilesController->Load(tempProfile, *computerSystemTypeInfo, *this)) {
                SetDialogValues(tempProfile);
            }

            return true;
        }

        case ID_SAVE_PROFILE: {
            Profile tempProfile;

            GetDialogValues(tempProfile);
            profilesController->Save(tempProfile, *this);

            return true;
        }

        case IDC_USEHEX:
        case IDC_SHOWZPASBYTE:
        case IDC_SBYTEALLOWED:
        case IDC_WORDALLOWED:
        case IDC_DSALLOWED:
        case IDC_INCLUDEALLOWED:
        case IDC_RADIOINCLUDE_ONE_FILE:
        case IDC_RADIOINCLUDE_ALL_FILES:
        case IDC_RADIOINCLUDE_NEXT_FILE:
            // Prevent recursion by triggering "value changed" events.
            if (HIWORD(wParam) == BN_CLICKED) {
                // Get current values and update enabled states.
                GetDialogValues(*profile);
                SetDialogValues(*profile);
            }
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    return false;
}
