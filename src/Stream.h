#pragma once

#include <iosfwd>
#include <iostream>

class Stream {
public:
    using stream_size = std::streamsize; // Can be negative!
    using stream_position = std::streampos;
    using stream_offset = std::streamoff;
};
