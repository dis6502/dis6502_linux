#pragma once

#include "Syntax.h"

/*
** File and file segment headers.
**
** See http://sdx.atari8.info/sdx_files/4.49/SDX449_Programming_Guide_EN.pdf for the deails of the SDX file formats.
*/
enum class FileHeader : unsigned short {
	RAW = 0x0000,
	ATARI_BINARY = 0xFFFF,
	SDX_FIXED_BLK = 0xFFFA,
	SDX_SYM_REQUIRED = 0xFFFB,
	SDX_SYM_DEFINED = 0xFFFC,
	SDX_FIX_UP_BLK = 0xFFFD,
	SDX_RELOC_BLK = 0xFFFE,
	ORIC_BINARY = 0x1616
};

wstring to_wstring(FileHeader fileHeader);