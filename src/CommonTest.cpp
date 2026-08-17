#include <cstring>
#include "CommonTest.h"
#include "Assert.h"
#include "ByteArray.h"
#include "DatatypeUtility.h"
#include "String.h"
#include <vector>
#include "Debug.h"

long Allocatable::instanceCount;

Allocatable::Allocatable() {
    instanceCount++;
    instanceNumber = instanceCount;
}

Allocatable::~Allocatable() {
    instanceCount--;
}

void Allocatable::Test() const {
    Assert::LogValue(L"instanceNumber", std::to_wstring(instanceNumber));
}

void Allocatable::Initialize() {
    instanceCount = 0;
}

void Allocatable::AssertAllDestroyed() {
    Assert::LongEquals(instanceCount, 0);
}

void CommonTest::TestCommon() {
    TestByteArray();
    TestDatatypeUtility();
    TestPointers();
    TestString();

}


void CommonTest::TestByteArray() {
    constexpr size_t SIZE = 10;
    constexpr size_t TARGET_SIZE = SIZE * 2;

    ByteArray a(SIZE);
    byte* buffer = new byte[SIZE];
    std::memset(buffer, 0, SIZE);
    ByteArray b(buffer, SIZE);
    Assert::LongEquals(a.size(), b.size());
    for (size_t i = 0; i < a.size(); i++) {
        Assert::ByteEquals(a.at(i), b.at(i));
    };

    byte target[TARGET_SIZE];
    a.copyTo(target, 0, TARGET_SIZE, 1, SIZE - 1);
}

void CommonTest::TestDatatypeUtility() {

    byte* byteValueArray;
    size_t outSize;
    Assert::BoolEquals(DatatypeUtility::ByteArrayFromHexString(byteValueArray, outSize, L""), false);
    Assert::BoolEquals(DatatypeUtility::ByteArrayFromHexString(byteValueArray, outSize, L"0"), false);
    Assert::BoolEquals(DatatypeUtility::ByteArrayFromHexString(byteValueArray, outSize, L"0x"), true);
    Assert::PointerNotNull(byteValueArray);
    Assert::LongEquals(outSize, 0);
    delete byteValueArray;
    Assert::BoolEquals(DatatypeUtility::ByteArrayFromHexString(byteValueArray, outSize, L"0x1"), false);
    Assert::BoolEquals(DatatypeUtility::ByteArrayFromHexString(byteValueArray, outSize, L"0x12"), true);
    Assert::PointerNotNull(byteValueArray);
    Assert::LongEquals(outSize, 1);
    delete byteValueArray;

    byte byteValue;
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L""), false);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"XYZ"), false);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"12"), true);
    Assert::LongEquals(byteValue, 12);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"0x13"), true);
    Assert::LongEquals(byteValue, 0x13);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"0x14"), true);
    Assert::LongEquals(byteValue, 0x14);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"0x100"), false);
    Assert::BoolEquals(DatatypeUtility::ByteFromString(byteValue, L"256"), false);

    Assert::StringEquals(DatatypeUtility::UnsignedLongHexString(1, true), L"0x1");

}

void CommonTest::TestString() {
    const auto text = String::Format(L"Start v0={0}, v1={1}, v2={2}, v3={3}, v4={4}, v5={5}, v6={6}, v7={7}, v8={8}, v9={9} End", L"A", L"AB", L"ABC", L"", L"E", L"EF", L"EFG", L"", L"X", L"XY");
    Assert::StringEquals(text, L"Start v0=A, v1=AB, v2=ABC, v3=, v4=E, v5=EF, v6=EFG, v7=, v8=X, v9=XY End");

    Assert::StringEquals(std::to_hex_string(0x0), L"0x0");
    Assert::StringEquals(std::to_hex_string(0x100), L"0x100");
    Assert::StringEquals(std::to_hex_string(0x100000), L"0x100000");

    Assert::StringEquals(std::to_byte_hex_string(0x00), L"0x00");
    Assert::StringEquals(std::to_byte_hex_string(0x0f), L"0x0f");
    Assert::StringEquals(std::to_byte_hex_string(0xff), L"0xff");
    Assert::StringEquals(std::to_byte_hex_string(0x100), L"value_out_of_byte_range");
}


void CommonTest::TestPointers() {
    Allocatable::Initialize();
    std::unique_ptr<Allocatable> a = std::make_unique<Allocatable>();
    a->Test();
    a.reset();
    Allocatable::AssertAllDestroyed();

    typedef std::vector<std::unique_ptr<Allocatable>> AllocatablelList;
    AllocatablelList list;
    list.push_back(std::make_unique<Allocatable>());
    list.push_back(std::make_unique<Allocatable>());
    list.push_back(std::make_unique<Allocatable>());

    for (auto&& b : list) {
        b->Test();
    }
    list.clear();
    Allocatable::AssertAllDestroyed();
}