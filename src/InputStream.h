#pragma once

#include "Syntax.h"
#include "Stream.h"

class InputStream: public Stream {
public:

	InputStream() = default;
	virtual ~InputStream() = default;  // Required, so sub-class destructor is called

    //virtual bool IsEOF() const = 0;
	virtual void Read(void* buffer, stream_size size) =0; // Throws IOException
	virtual void Skip(stream_offset offset) =0; // Throws IOException
    virtual stream_position GetPosition() = 0 ; // Returns -1 if there is no valid position
};