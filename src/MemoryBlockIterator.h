#pragma once

#include "Syntax.h"
#include "Byte.h"
#include "MemoryType.h"
#include "Memory.h"

class MemoryBlock;

class MemoryBlockIterator {
public:

    MemoryBlockIterator(const MemoryBlock& memoryBlock);
    bool HasNext() const;
    byte GetData() const;
    byte NextData();
    Memory::address NextAddress();
    MemoryType GetType() const;
    void Next();
private:
    const MemoryBlock* memoryBlock;
    size_t index;
};