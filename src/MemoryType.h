#pragma once

#include "Syntax.h"
#include "Byte.h"

/*
** Original sequence from version 2.2
 */

namespace version_22 {
	enum class MemoryType : byte {
		UNKNOWN = 0,
		LOBYTE,
		HIBYTE,
		BYTE,
		WORD,
		LABEL,
		STRING,
		SBYTE,
		DLIST,
		STORE,
		CODE
	};
}

/*
** Type of bytes found in type buffer that qualify the memoryInspector buffer.
** MemoryType::LOBYTE and MemoryType::HIBYTE are followed by a byte that
** represents the missing part of the address. For example, if we have
** $A9 $04 in the source (and it sould be disasembled as LDA # >L0480),
** we will have MemoryType::HIBYTE followed by $80 in the type buffer to
** say that we have a HIBYTE ($04) in the memoryInspector buffer and the LOBYTE is
** $80.
*/
enum class MemoryType : byte {
	UNKNOWN = 0,
	LOBYTE,
	HIBYTE,
	BYTE,
	WORD,
	LABEL,
	STRING,
	SBYTE,
	DLIST,
	STORE,
	CODE,
	SYMBOL,             // New in version 3.0
	FIXUP,              // New in version 3.0
	// LOTABLE,            // New in version 3.0, not implemented yet
	// HITABLE,            // New in version 3.0, not implemented yet
};

constexpr int MEMORY_TYPE_ENUM_ITEM_COUNT = static_cast<byte>(MemoryType::FIXUP)+1;
