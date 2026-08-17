#include <cstring>
#include "PlatformCompat.h"
#include "Syntax.h"

#include <sstream>
#include <span>
#include "ByteArray.h"
#include "DatatypeUtility.h"

ByteArray::ByteArray() : ByteArray(size_t{ 0 }) {

}

ByteArray::ByteArray(size_t size) : ByteArray(new byte[size], size) {
    memset(_array, 0, _size);
}

ByteArray::ByteArray(byte array[], size_t size) :
    _array(array), _size(size) {

}

ByteArray::ByteArray(const ByteArray& array) : ByteArray(new byte[array._size], array._size) {
    memcpy(_array, array._array, _size);
}

ByteArray::ByteArray(ByteArray&& array) noexcept : ByteArray(new byte[array._size], array._size) {
    memcpy(_array, array._array, _size);
}

ByteArray& ByteArray::operator=(const ByteArray& other) {
    return AssignFrom(other);
}

ByteArray& ByteArray::operator=(ByteArray&& other) noexcept {
    return AssignFrom(other);
}

ByteArray& ByteArray::AssignFrom(const ByteArray& other) {
    // Guard self assignment
    if (this == &other) {
        return *this; // delete[]/size=0 would also be OK
    }
    // assume *this manages a reusable resource, such as a heap-allocated buffer array
    if (_size != other._size) {         // resource in *this cannot be reused
        delete[] _array;                // release resource in *this

        _array = nullptr;
        _size = 0;                      // preserve invariants in case next line throws
        _array = new byte[other._size]; // allocate resource in *this
        _size = other._size;
    }

    std::copy(other._array, other._array + other._size, _array);
    return *this;
}

ByteArray::~ByteArray() {
    delete[] _array;
}

byte* ByteArray::get() {
    return _array;
}

const byte* ByteArray::getConst() const {
    return _array;
}

bool ByteArray::empty() const {
    return (_size == 0);
}

size_t ByteArray::size() const {
    return _size;
}

byte ByteArray::at(size_t index) const {
    if (index < _size) {
        return _array[index];
    }
    throw std::runtime_error((std::stringstream() << "Array index " << index << " is larger than " << (_size - 1)).str().c_str());
}

void ByteArray::copyTo(byte* target, size_t targetOffset, size_t targetSize, size_t offset, size_t size) const {
    if (offset + size > _size) {
        throw std::runtime_error("Source offset plus size exceed size of this byte array");
    }
    if (targetOffset + size > targetSize) {
        throw std::runtime_error("Target offset plus size exceed size of target byte array");
    }
    std::memcpy(target + targetOffset, _array + offset, size);
}

std::unique_ptr<ByteSequence> ByteArray::GetSubSequence(size_t offset, size_t size) const {
    if (offset >= _size) {
        return std::make_unique<ByteArray>();
    }

    auto copySize = std::min((size_t)(_size - offset), size);
    auto resultArray = new byte[copySize];
    memcpy(resultArray, _array + offset, copySize);
    return std::make_unique<ByteArray>(resultArray, copySize);
}

string ByteArray::stringAt(size_t offset, size_t size) const {
    if (offset >= _size) {
        return "";
    }
    auto buffer = new char[size + 1];
    auto resultSize = std::min((size_t)(_size - offset), size);
    _memccpy(buffer, (const void*)_array, offset, resultSize);
    buffer[resultSize] = 0;
    auto result = string(buffer);
    delete[] buffer;
    return result;
}

wstring ByteArray::to_wstring() const {
    return DatatypeUtility::ByteArrayToHexString(_array, _size, true);
}

void ByteArray::setAt(size_t index, byte value) {
    if (index >= _size) {
        throw std::runtime_error((std::stringstream() << "Array index " << index << " is larger than " << (_size - 1)).str().c_str());
    }
    _array[index] = value;
}

void ByteArray::setAt(size_t index, byte value, size_t size) {
    if (index >= _size) {
        throw std::runtime_error((std::stringstream() << "Array index " << index << " is larger than " << (_size - 1)).str().c_str());
    }
    if (index + size - 1 >= _size) {
        throw std::runtime_error((std::stringstream() << "Array index " << index << " plus size " << size << " is larger than " << (_size - 1)).str().c_str());
    }
    std::memset(&_array[index], value, size);

}
