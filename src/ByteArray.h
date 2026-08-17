#pragma once

#include "Syntax.h"

#include "Byte.h"
#include "ByteSequence.h"

// A ByteArray is a fixed-size modifiable non-const ower of a byte[].
// Creating a ByteArray from a byte[] takes over the ownership of the byte [],
// Copying the ByteArray also copies the underlying byte[]
class ByteArray final : public ByteSequence {

public:
    ByteArray(); // Creates own empty byte[]
    ByteArray(size_t size); // Creates own byte[]
    ByteArray(byte array[], size_t size); // Takes over ownership of the byte[]
    ByteArray(const ByteArray& array); // Creates copy of byte[]
    ByteArray(ByteArray&&) noexcept; // Creates copy of byte[]
    ByteArray& operator=(const ByteArray& other); // Creates copy of byte[]
    ByteArray& operator=(ByteArray&&) noexcept;  // Creates copy of byte[]

    ~ByteArray();


    byte* get() override;
    const byte* getConst() const override;
    bool empty() const override;
    size_t size() const override;
    byte at(size_t index) const override;
    // targetOffset is between 0 and <targetSize
    // targetSize is the total size of the target, irrespective of the targetOffset
    void copyTo(byte* target, size_t targetOffset, size_t targetSize, size_t offset, size_t size) const override;
    std::unique_ptr<ByteSequence> GetSubSequence(size_t offset, size_t size) const override;
    string stringAt(size_t offset, size_t size) const override;
    wstring to_wstring() const override;

    void setAt(size_t index, byte value);
    void setAt(size_t index, byte value, size_t size);


private:
    byte* _array;
    size_t _size;

    ByteArray& AssignFrom(const ByteArray& other);
};