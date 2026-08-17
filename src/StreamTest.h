#pragma once

#include "Stream.h"

class InputStream;

class StreamTest {

public:
    static void AssertInputStreamsEqual(InputStream& actualInputStream, InputStream& expectedInputStream, Stream::stream_size expectedStreamSize);

};