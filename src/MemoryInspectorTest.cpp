#include "MemoryType.h"

#include "MemoryInspectorTest.h"


void MemoryInspectorTest::Test()
{
    TestMemoryInspectorType();
}

void MemoryInspectorTest::TestMemoryInspectorType() {
    // Ensure binary compatibility of the type encoding.
    Assert::ByteEquals((byte)version_22::MemoryType::UNKNOWN, (byte)MemoryType::UNKNOWN);
    Assert::ByteEquals((byte)version_22::MemoryType::LOBYTE, (byte)MemoryType::LOBYTE);
    Assert::ByteEquals((byte)version_22::MemoryType::HIBYTE, (byte)MemoryType::HIBYTE);
    Assert::ByteEquals((byte)version_22::MemoryType::BYTE, (byte)MemoryType::BYTE);
    Assert::ByteEquals((byte)version_22::MemoryType::WORD, (byte)MemoryType::WORD);
    Assert::ByteEquals((byte)version_22::MemoryType::LABEL, (byte)MemoryType::LABEL);
    Assert::ByteEquals((byte)version_22::MemoryType::STRING, (byte)MemoryType::STRING);
    Assert::ByteEquals((byte)version_22::MemoryType::SBYTE, (byte)MemoryType::SBYTE);
    Assert::ByteEquals((byte)version_22::MemoryType::DLIST, (byte)MemoryType::DLIST);
    Assert::ByteEquals((byte)version_22::MemoryType::STORE, (byte)MemoryType::STORE);
    Assert::ByteEquals((byte)version_22::MemoryType::CODE, (byte)MemoryType::CODE);
}
