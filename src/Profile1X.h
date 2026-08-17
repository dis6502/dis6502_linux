/*
 * Profile1X.h
 *
 * Type definitions for profile version from 1.0 to 1.7.
 * Starting with version 3.0, the extensible XML format is used instead.
 */
#pragma once

#include "Syntax.h"

#include <cstdint>

#include "Byte.h"
#include "ByteSequence.h"

class Profile;

constexpr size_t MAX_PROFILE1X_STRING_LNG = 20;
typedef char PROFILE1X_STRING[MAX_PROFILE1X_STRING_LNG];
typedef int32_t PROFILE1X_BOOL; // was BOOL
typedef uint16_t PROFILE1X_WORD; // was WORD

#define PROFILE10_FIELDS \
/* General */ \
PROFILE1X_STRING	commentPrefix; \
PROFILE1X_STRING	hexNotationPrefix; \
PROFILE1X_BOOL		useIllegalOpcodes; \
PROFILE1X_BOOL		useLineNumbers; \
PROFILE1X_BOOL		alignInstructions; \
PROFILE1X_BOOL		useHexNotation; \
PROFILE1X_BOOL		showAInAccumulatorMode; \
/* Assembler syntax */ \
PROFILE1X_STRING	directiveBYTE; \
PROFILE1X_STRING	directiveWORD; \
PROFILE1X_STRING	directiveSBYTE; \
PROFILE1X_STRING	directiveORG; \
PROFILE1X_STRING	directiveEQU; \
PROFILE1X_STRING	directiveENDHead; \
PROFILE1X_STRING	directiveENDTail; \
PROFILE1X_STRING	directiveLOWHead; \
PROFILE1X_STRING	directiveLOWTail; \
PROFILE1X_STRING	directiveHIGHHead; \
PROFILE1X_STRING	directiveHIGHTail; \
PROFILE1X_STRING	directiveBYTESeparator; \
PROFILE1X_BOOL		directiveBYTEOnlyNumbersAllowed; \
PROFILE1X_BOOL		directiveWORDAllowed; \
PROFILE1X_BOOL		directiveSBYTEAllowed; \
PROFILE1X_BOOL		directiveENDNeedsFilename; \
/* Include and object files */ \
PROFILE1X_STRING	directiveINCLUDEHead; \
PROFILE1X_STRING	directiveINCLUDETail; \
PROFILE1X_WORD		directiveINCLUDEMaximumNumberOfLinesPerFile; \
PROFILE1X_BOOL		directiveINCLUDEAllIncludesInMainFile; \
PROFILE1X_BOOL		directiveINCLUDEAllowed;


/*
** structures
*/
typedef struct PROFILE10 {
	PROFILE10_FIELDS
} PROFILE10;

#define PROFILE13_FIELDS \
/* Addition to assembler syntax */\
PROFILE1X_STRING    directiveDS;\
PROFILE1X_BOOL	    directiveDSAllowed;

typedef struct PROFILE13 {
	PROFILE10_FIELDS
		PROFILE13_FIELDS
} PROFILE13;

#define PROFILE16_FIELDS \
/* Addition to general */ \
PROFILE1X_BOOL		showBRKAsByte0; \
PROFILE1X_BOOL		showLowerCaseInstructions; \
PROFILE1X_BOOL		showColonAfterLabel;

typedef struct PROFILE16 {
	PROFILE10_FIELDS
		PROFILE13_FIELDS
		PROFILE16_FIELDS
} PROFILE16;

#define PROFILE17_FIELDS \
/* Addition to general */ \
PROFILE1X_BOOL	    showZPAbsoluteAsByte;

typedef struct PROFILE17 {
	PROFILE10_FIELDS
	PROFILE13_FIELDS
	PROFILE16_FIELDS
	PROFILE17_FIELDS
} PROFILE17;


class Profile1X {
public:
	static bool Load(Profile& profile, const ByteSequence& buffer);
};
