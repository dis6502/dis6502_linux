#pragma once

#include "Syntax.h"

enum class PrintfState {
	TEXT = 0,
	PERCENT,
	FORMAT,
};

class Symbol;
class Fixup;
class Address;
class Label;
class Segment;

typedef short SEGMENT_INDEX; // 0...SEGMENT_COUNT-1
typedef short SEGMENT_NUMBER; // 1..SEGMENT_COUNT
typedef short SEGMENT_COUNT;

constexpr SEGMENT_INDEX NO_SEGMENT_INDEX = -1;
