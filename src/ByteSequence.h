#pragma once

#include "Syntax.h"

#include "Byte.h"

class ByteSequence {
public:

    virtual ~ByteSequence() = default;

    virtual byte* get() = 0;
    virtual const byte* getConst() const = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
    virtual byte at(size_t index) const = 0;
    virtual void copyTo(byte* target, size_t targetOffset, size_t targetSize, size_t offset, size_t size) const = 0;

    virtual std::unique_ptr<ByteSequence> GetSubSequence(size_t offset, size_t size) const = 0;
    virtual string stringAt(size_t offset, size_t size) const = 0;
    virtual wstring to_wstring() const = 0;
};