#include "MemoryBlockIterator.h"
#include "MemoryBlock.h"

MemoryBlockIterator::MemoryBlockIterator(const MemoryBlock& memoryBlock) :
    memoryBlock(&memoryBlock),
    index(0) {
}

bool MemoryBlockIterator::HasNext() const {
    return (index < memoryBlock->GetSize());
}

byte MemoryBlockIterator::NextData() {
    const auto result = GetData();
    Next();
    return result;
}

Memory::address MemoryBlockIterator::NextAddress() {
    const auto low = GetData();
    Next();
    const auto high = GetData();
    Next();
    return Memory::to_address(low, high);
}

byte MemoryBlockIterator::GetData() const {
    return memoryBlock->GetDataAt(index);
}

MemoryType MemoryBlockIterator::GetType() const {
    return memoryBlock->GetTypeAt(index);
}

void MemoryBlockIterator::Next() {
    if (!HasNext()) {
        throw std::runtime_error("End of memory block reached");
    }
    index++;
}