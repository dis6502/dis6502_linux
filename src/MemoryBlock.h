#pragma once
#include "Syntax.h"
#include "XML.h"
#include "Memory.h"
#include "MemoryType.h"
#include "Byte.h"
#include "ByteArray.h"
#include "ByteSequence.h"
#include <gsl/gsl>

class InputStream;
class OutputStream;
// TODO: Friends to be removed
class Disassembly;
class MemoryInspector;

class MemoryBlock : public XML::Serializable {
public:
    MemoryBlock();
    MemoryBlock(const MemoryBlock& a) = delete;			  // copy constructor
    MemoryBlock& operator=(const MemoryBlock&) = delete;  // copy assignment
    MemoryBlock(MemoryBlock&&) = delete;                  // move constructor
    MemoryBlock& operator=(MemoryBlock&&) = delete;       // move assignment
    virtual ~MemoryBlock() = default;

    void Create(Memory::size size);
    void Clear();
    Memory::size GetSize() const;
    bool IsEmpty() const;

    gsl::not_null<ByteSequence*> GetMutableData();

    gsl::not_null<const ByteSequence*> GetData() const;

    inline byte GetDataAt(Memory::offset offset) const {
        return data.at(offset);
    }

    inline void SetDataAt(Memory::offset offset, byte data) {
        this->data.setAt(offset, data);
    }

    void SetDataAt(Memory::offset offset, const ByteSequence& data, size_t dataOffset, Memory::size dataSize);

    void ReadData(InputStream& inputStream, Memory::size); // Throws IOException
    void WriteData(OutputStream& outputStream) const; // Throws IOException

    gsl::not_null<ByteSequence*> GetMutableType();

    gsl::not_null<const ByteSequence*> GetType() const;

    inline MemoryType GetTypeAt(Memory::offset offset) const {
        return (MemoryType)type.at(offset);
    }

    inline void SetTypeAt(Memory::offset offset, MemoryType memoryType) {
        this->type.setAt(offset, (byte)memoryType);
    }

    void SetTypeAt(Memory::offset offset, MemoryType memoryType, Memory::size size);
    void ReadType(InputStream& inputStream, Memory::size size);
    void WriteType(OutputStream& outputStream) const;

    void CopyTo(Memory::offset startOffset, Memory::size size, MemoryBlock& target, Memory::offset targetOffset);

    void SerializeTo(XML::Element& lpElement) const override;
    void DeserializeFrom(const XML::Element& lpElement) override;

private:
    friend class Disassembly; // TODO Remove
    friend class MemoryInspector; // TODO Remove

    Memory::size size;
    ByteArray data;
    byte* lpData;
    ByteArray type;
    byte* lpType;


};