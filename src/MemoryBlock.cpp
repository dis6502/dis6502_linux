#include "MemoryBlock.h"
#include "InputStream.h"
#include "OutputStream.h"
#include <gsl/gsl>
#include <sstream>
MemoryBlock::MemoryBlock() :
    size(0),
    lpData(nullptr),
    lpType(nullptr) {

}

void MemoryBlock::Create(Memory::size size) {
    this->size = size;
    data = ByteArray(size);
    type = ByteArray(size);

    lpData = data.get();
    lpType = type.get();
    data.setAt(0, 0x00, size);
    type.setAt(0, static_cast<byte>(MemoryType::UNKNOWN), size);
}


void MemoryBlock::Clear() {
    data = ByteArray();
    type = ByteArray();

    lpData = nullptr;
    lpType = nullptr;

    size = 0;
}

Memory::size MemoryBlock::GetSize() const {
    return size;
}


bool MemoryBlock::IsEmpty() const {
    return (size == 0);
}

gsl::not_null<ByteSequence*> MemoryBlock::GetMutableData() {
    return (gsl::not_null<ByteSequence*>) & data;
}


gsl::not_null<const ByteSequence*> MemoryBlock::GetData() const {
    return (gsl::not_null<const ByteSequence*>) & data;
}

void MemoryBlock::SetDataAt(Memory::offset offset, const ByteSequence& data, size_t dataOffset, Memory::size dataSize) {
    for (Memory::size i = 0; i < dataSize; i++) {
        SetDataAt(offset + i, data.at(dataOffset + i));
    }
}

void MemoryBlock::ReadData(InputStream& inputStream, Memory::size size) {
    inputStream.Read(lpData, size);
}

void MemoryBlock::WriteData(OutputStream& outputStream) const {
    outputStream.Write(data.getConst(), data.size());
}

gsl::not_null<ByteSequence*> MemoryBlock::GetMutableType() {
    return (gsl::not_null<ByteSequence*>) & type;

}

gsl::not_null<const ByteSequence*> MemoryBlock::GetType() const {
    return (gsl::not_null<const ByteSequence*>) & type;
}


void MemoryBlock::SetTypeAt(Memory::offset offset, MemoryType memoryType, Memory::size size) {
    type.setAt(offset, static_cast<byte>(memoryType), size);
}


void MemoryBlock::ReadType(InputStream& inputStream, Memory::size size) {
    inputStream.Read(lpType, size);
}

void MemoryBlock::WriteType(OutputStream& outputStream) const {
    outputStream.Write(type.getConst(), type.size());
}

void MemoryBlock::CopyTo(Memory::offset startOffset, Memory::size size, MemoryBlock& target, Memory::offset targetOffset) {

    data.copyTo(target.lpData, targetOffset, target.GetSize(), startOffset, size);
    type.copyTo(target.lpType, targetOffset, target.GetSize(), startOffset, size);
}

void MemoryBlock::SerializeTo(XML::Element& element) const {
    SetSizeAttributeHex(Size, size);
    SetByteArrayAttributeHex(Data, lpData, size);
    SetByteArrayAttributeHex(Type, lpType, size);
}

void MemoryBlock::DeserializeFrom(const XML::Element& element) {

    Clear();
    size_t size = 0;
    GetSizeAttribute(Size, size);
    size_t realSize = 0;

    byte* lpData = nullptr;
    // See GetByteArrayAttribute(Data, lpData, nRealSize);
    // Format in version 3.6 was <Content Size="7484" Dump="0xD8ADC200AEC3" Type="0x00000C0C000C0C000B0B0000000">
    auto attribute = element.Attribute("Data");
    if (attribute == nullptr) {
        attribute = element.Attribute("Dump");  // For compatibility with 3.6
    }
    if (attribute == nullptr) {
        std::stringstream buffer;
        buffer << "Attribute \"Data\" of memory block is missing for element " << element.Name() << " in document line " << element.GetLineNum();
        throw std::runtime_error(buffer.str());
    }

    if (!XML::ByteArrayFromUTF8String(lpData, realSize, attribute)) {
        throw std::runtime_error("Cannot read byte array from string");
    }
    if (realSize != size) {
        throw std::runtime_error("Size of content array is different from size of memory block");
    }

    byte* lpType = nullptr;
    GetByteArrayAttribute(Type, lpType, realSize);
    if (realSize != size) {
        throw std::runtime_error("Size of type array is different from size of memory block");
    }

    Create(size);
    memcpy(this->lpData, lpData, realSize);
    memcpy(this->lpType, lpType, realSize);

    delete lpData;
    delete lpType;
}
