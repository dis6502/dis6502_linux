#include "Profile.h"


Profile::Profile() noexcept {
    Clear();
}

void Profile::Clear() {
    // Preset default this with MADS settings.
    commentPrefix = L";";
    hexNotationPrefix = L"$";
    useIllegalOpcodes = false;
    useLineNumbers = false;
    alignInstructions = true;
    useHexNotation = true;
    showAInAccumulatorMode = false;
    showBRKAsByte0 = true;
    showLowerCaseInstructions = true;
    showColonAfterLabel = false;
    showZPAbsoluteAsByte = false;
    directiveForceAbsolute = L".w";
    quoteForASCIIStrings = L"\'";
    directiveWORDNumberOfWordsPerLine = 1; // Because WORDs often mean labels and labels now have "SnnnLxxxx" format
    directiveBYTENumberOfBytesPerLine = 16;
    directiveBYTENumberOfCharactersPerString = 40;
    showOpcodeAsComment = false;

    // Directive syntax.
    directiveBYTE = L".byte";
    directiveWORD = L".word";
    directiveSBYTE = L".sb";
    directiveORG = L"org";
    directiveEQU = L"equ";
    directiveENDHead = L"";
    directiveENDTail = L"";
    directiveLOWHead = L"<";
    directiveLOWTail = L"";
    directiveHIGHHead = L">";
    directiveHIGHTail = L"";
    directiveBYTESeparator = L",";
    directiveBYTEOnlyNumbersAllowed = false;
    directiveWORDAllowed = true;
    directiveSBYTEAllowed = true;
    directiveENDNeedsFilename = false;
    showNonASCIIChararactersAsBytes = true;

    directiveDSAllowed = true;
    directiveDS = L".ds";

    // Disassembly listing.
    outputEncoding = Encoding::ASCII;
    omitUnreferencedSystemLabels = true;

    // Include files.
    directiveINCLUDEAllowed = true;
    directiveINCLUDEHead = L"icl '";
    directiveINCLUDETail = L"'";
    directiveINCLUDEAllEquatesInOneIncludeFile = true;
    directiveINCLUDEAllIncludesInMainFile = false;
    directiveINCLUDEMaximumNumberOfLinesPerFile = 0;
}

void Profile::SerializeTo(XML::Element& element) const {
    // Source Layout
    SetBoolAttribute(UseLineNumbers, useLineNumbers);
    SetBoolAttribute(AlignInstructions, alignInstructions);
    SetBoolAttribute(ShowLowerCaseInstructions, showLowerCaseInstructions);

    // Opcodes
    SetBoolAttribute(UseIllegalOpcodes, useIllegalOpcodes);
    SetBoolAttribute(ShowAInAccumulatorMode, showAInAccumulatorMode);
    SetBoolAttribute(ShowColonAfterLabel, showColonAfterLabel);
    SetBoolAttribute(ShowOpcodeAsComment, showOpcodeAsComment);
    SetBoolAttribute(ShowBRKAsByte0, showBRKAsByte0);
    SetBoolAttribute(ShowZPAbsoluteAsByte, showZPAbsoluteAsByte);
    SetStdStringAttribute(DirectiveForceAbsolute, directiveForceAbsolute);

    // Comments
    SetStdStringAttribute(CommentPrefix, commentPrefix);

    // Numbers
    SetBoolAttribute(UseHexNotation, useHexNotation);
    SetStdStringAttribute(HexNotationPrefix, hexNotationPrefix);

    // Strings
    SetBoolAttribute(ShowNonASCIIChararactersAsBytes, showNonASCIIChararactersAsBytes);
    SetStdStringAttribute(QuoteForASCIIStrings, quoteForASCIIStrings);

    // Directives
    SetStdStringAttribute(DirectiveLOWHead, directiveLOWHead);
    SetStdStringAttribute(DirectiveLOWTail, directiveLOWTail);
    SetStdStringAttribute(DirectiveHIGHHead, directiveHIGHHead);
    SetStdStringAttribute(DirectiveHIGHTail, directiveHIGHTail);

    SetStdStringAttribute(DirectiveBYTE, directiveBYTE);
    SetStdStringAttribute(DirectiveBYTESeparator, directiveBYTESeparator);
    SetWordAttribute(DirectiveBYTENumberOfBytesPerLine, directiveBYTENumberOfBytesPerLine);
    SetWordAttribute(DirectiveBYTENumberOfCharactersPerString, directiveBYTENumberOfCharactersPerString);
    SetBoolAttribute(DirectiveBYTEOnlyNumbersAllowed, directiveBYTEOnlyNumbersAllowed);
    SetBoolAttribute(DirectiveSBYTEAllowed, directiveSBYTEAllowed);
    SetStdStringAttribute(DirectiveSBYTE, directiveSBYTE);
    SetBoolAttribute(DirectiveWORDAllowed, directiveWORDAllowed);
    SetStdStringAttribute(DirectiveWORD, directiveWORD);
    SetWordAttribute(DirectiveWORDNumberOfWordsPerLine, directiveWORDNumberOfWordsPerLine);
    SetBoolAttribute(DirectiveDSAllowed, directiveDSAllowed);
    SetStdStringAttribute(DirectiveDS, directiveDS);

    // Source structure
    SetStdStringAttribute(DirectiveORG, directiveORG);
    SetStdStringAttribute(DirectiveEQU, directiveEQU);
    SetStdStringAttribute(DirectiveENDHead, directiveENDHead);
    SetStdStringAttribute(DirectiveENDTail, directiveENDTail);
    SetBoolAttribute(DirectiveENDNeedsFilename, directiveENDNeedsFilename);

    // Disassembly listing
    wstring outputEncoding = EncodingFactory::GetInfo(this->outputEncoding).key;
    SetStdStringAttribute(OutputEncoding, outputEncoding);
    SetBoolAttribute(OmitUnreferencedSystemLabels, omitUnreferencedSystemLabels);

    // Include files
    SetBoolAttribute(DirectiveINCLUDEAllowed, directiveINCLUDEAllowed);
    SetStdStringAttribute(DirectiveINCLUDEHead, directiveINCLUDEHead);
    SetStdStringAttribute(DirectiveINCLUDETail, directiveINCLUDETail);
    SetBoolAttribute(DirectiveINCLUDEAllEquatesInOneIncludeFile, directiveINCLUDEAllEquatesInOneIncludeFile);
    SetBoolAttribute(DirectiveINCLUDEAllIncludesInMainFile, directiveINCLUDEAllIncludesInMainFile);
    SetWordAttribute(DirectiveINCLUDEMaximumNumberOfLinesPerFile, directiveINCLUDEMaximumNumberOfLinesPerFile);
}

void Profile::DeserializeFrom(const XML::Element& element) {
    Clear();

    // Source Layout
    GetBoolAttribute(UseLineNumbers, useLineNumbers);
    GetBoolAttribute(AlignInstructions, alignInstructions);
    GetBoolAttribute(ShowLowerCaseInstructions, showLowerCaseInstructions);

    // Opcodes
    GetBoolAttribute(UseIllegalOpcodes, useIllegalOpcodes);
    GetBoolAttribute(ShowAInAccumulatorMode, showAInAccumulatorMode);
    GetBoolAttribute(ShowColonAfterLabel, showColonAfterLabel);
    GetBoolAttribute(ShowOpcodeAsComment, showOpcodeAsComment);
    GetBoolAttribute(ShowBRKAsByte0, showBRKAsByte0);
    GetBoolAttribute(ShowZPAbsoluteAsByte, showZPAbsoluteAsByte);
    GetStdStringAttribute(DirectiveForceAbsolute, directiveForceAbsolute);

    // Comments
    GetStdStringAttribute(CommentPrefix, commentPrefix);

    // Numbers
    GetBoolAttribute(UseHexNotation, useHexNotation);
    GetStdStringAttribute(HexNotationPrefix, hexNotationPrefix);

    // Strings
    GetBoolAttribute(ShowNonASCIIChararactersAsBytes, showNonASCIIChararactersAsBytes);
    GetStdStringAttribute(QuoteForASCIIStrings, quoteForASCIIStrings);

    // Directives
    GetStdStringAttribute(DirectiveLOWHead, directiveLOWHead);
    GetStdStringAttribute(DirectiveLOWTail, directiveLOWTail);
    GetStdStringAttribute(DirectiveHIGHHead, directiveHIGHHead);
    GetStdStringAttribute(DirectiveHIGHTail, directiveHIGHTail);

    GetStdStringAttribute(DirectiveBYTE, directiveBYTE);
    GetStdStringAttribute(DirectiveBYTESeparator, directiveBYTESeparator);
    GetWordAttribute(DirectiveBYTENumberOfBytesPerLine, directiveBYTENumberOfBytesPerLine);
    GetWordAttribute(DirectiveBYTENumberOfCharactersPerString, directiveBYTENumberOfCharactersPerString);
    GetBoolAttribute(DirectiveBYTEOnlyNumbersAllowed, directiveBYTEOnlyNumbersAllowed);
    GetBoolAttribute(DirectiveSBYTEAllowed, directiveSBYTEAllowed);
    GetStdStringAttribute(DirectiveSBYTE, directiveSBYTE);
    GetBoolAttribute(DirectiveWORDAllowed, directiveWORDAllowed);
    GetStdStringAttribute(DirectiveWORD, directiveWORD);
    GetWordAttribute(DirectiveWORDNumberOfWordsPerLine, directiveWORDNumberOfWordsPerLine);
    GetBoolAttribute(DirectiveDSAllowed, directiveDSAllowed);
    GetStdStringAttribute(DirectiveDS, directiveDS);

    // Source Structure
    GetStdStringAttribute(DirectiveORG, directiveORG);
    GetStdStringAttribute(DirectiveEQU, directiveEQU);
    GetStdStringAttribute(DirectiveENDHead, directiveENDHead);
    GetStdStringAttribute(DirectiveENDTail, directiveENDTail);
    GetBoolAttribute(DirectiveENDNeedsFilename, directiveENDNeedsFilename);

    // Disassembly listing
    wstring outputEncoding;
    GetStdStringAttribute(OutputEncoding, outputEncoding);
    this->outputEncoding = EncodingFactory::GetInfo(outputEncoding).encoding;

    // Ignore unsuitable encodings.
    switch (this->outputEncoding) {
    case Encoding::ASCII:
    case Encoding::ATASCII:
    case Encoding::UTF8:
        break;
    default:
        this->outputEncoding = Encoding::ASCII;

    }
    GetBoolAttribute(OmitUnreferencedSystemLabels, omitUnreferencedSystemLabels);

    // Include files
    GetBoolAttribute(DirectiveINCLUDEAllowed, directiveINCLUDEAllowed);
    GetStdStringAttribute(DirectiveINCLUDEHead, directiveINCLUDEHead);
    GetStdStringAttribute(DirectiveINCLUDETail, directiveINCLUDETail);
    GetBoolAttribute(DirectiveINCLUDEAllEquatesInOneIncludeFile, directiveINCLUDEAllEquatesInOneIncludeFile);
    GetBoolAttribute(DirectiveINCLUDEAllIncludesInMainFile, directiveINCLUDEAllIncludesInMainFile);
    GetWordAttribute(DirectiveINCLUDEMaximumNumberOfLinesPerFile, directiveINCLUDEMaximumNumberOfLinesPerFile);
}
