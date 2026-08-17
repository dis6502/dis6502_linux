#pragma once

#include "Syntax.h"
#include "IO.h"
#include <iostream>

class Stream {
public:
	using stream_size = std::streamsize; // Can be negative
    using stream_position = std::streampos;
	using stream_offset = std::streamoff;
};
