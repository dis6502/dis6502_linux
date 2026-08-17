#pragma once

#include "Syntax.h"
#include "Word.h"
#include "XML.h"

class Profile: public XML::Serializable {
public:
	/* Source Layout */
	bool			useLineNumbers; // since 1.0
	bool			alignInstructions; // since 1.0
	bool			showLowerCaseInstructions;  // since 1.6

	/* Opcodes */
	bool			useIllegalOpcodes; // since 1.0
	bool			showAInAccumulatorMode; // since 1.0
	bool			showColonAfterLabel;  // since 1.6
	bool			showOpcodeAsComment;  // since 3.0
	bool			showBRKAsByte0; // since 1.6
	bool			showZPAbsoluteAsByte; // since 1.7
	wstring			directiveForceAbsolute; // since 3.6

	/* Comments */
	wstring			commentPrefix; // since 1.0

	/* Numbers */
	bool			useHexNotation; // since 1.0
	wstring			hexNotationPrefix; // since 1.0

	/* Strings */
	bool			showNonASCIIChararactersAsBytes; // since 3.0
	wstring			quoteForASCIIStrings;  // since 3.0, string where only the first character is used

	/* Directives */
	wstring			directiveLOWHead; // since 1.0
	wstring			directiveLOWTail; // since 1.0
	wstring			directiveHIGHHead; // since 1.0
	wstring			directiveHIGHTail; // since 1.0

	wstring			directiveBYTE; // since 1.0
	wstring			directiveBYTESeparator; // since 1.0
	word            directiveBYTENumberOfBytesPerLine;  // since 3.0
	word            directiveBYTENumberOfCharactersPerString;   // since 3.0
	bool			directiveBYTEOnlyNumbersAllowed; // since 1.0
	bool			directiveSBYTEAllowed; // since 1.0
	wstring			directiveSBYTE; // since 1.0
	bool			directiveWORDAllowed; // since 1.0
	wstring			directiveWORD; // since 1.0
	word            directiveWORDNumberOfWordsPerLine;  // since 3.0
	bool			directiveDSAllowed; // since 1.3
	wstring			directiveDS; // since 1.3

	/* Source Structure */
	wstring			directiveORG; // since 1.0
	wstring			directiveEQU; // since 1.0
	wstring			directiveENDHead; // since 1.0
	bool			directiveENDNeedsFilename;
	wstring			directiveENDTail; // since 1.0

	/* Disassembly Listing */
    Encoding        outputEncoding; // since 4.0
	bool			omitUnreferencedSystemLabels;  // since 3.0
	bool			directiveINCLUDEAllowed; // since 1.0
	wstring			directiveINCLUDEHead; // since 1.0
	wstring			directiveINCLUDETail;  // since 1.0
	bool			directiveINCLUDEAllEquatesInOneIncludeFile;  // since 3.0
	bool			directiveINCLUDEAllIncludesInMainFile; // since 1.0
	word            directiveINCLUDEMaximumNumberOfLinesPerFile;  // since 1.0


	Profile() noexcept;
	void Clear();

	void SerializeTo(XML::Element& element) const override;
	void DeserializeFrom(const XML::Element& element) override;
};
