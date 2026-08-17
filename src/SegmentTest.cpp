#include "SegmentTest.h"
#include "Segment.h"
#include "AddressLabel.h"
#include "AddressLabelList.h"
#include "Assertions.h"

void SegmentTest::TestSegment() {

    Segment segment;
    Assert::LongEquals(segment.GetSize(), 0);
    Assert::BoolEquals(segment.IsEmpty(), true);

    segment.wBegin = 0x2000;
    segment.wEnd = 0x3fff;
    Assert::LongEquals(segment.GetSize(), 0);

    segment.CreateMemoryBlockFromBeginToEnd();
    Assert::LongEquals(segment.GetSize(), 0x2000);


    Memory::offset offset = 0;
    segment.SetData(offset++, 0x020);
    segment.SetData(offset++, 0x004);
    segment.SetData(offset++, 0x020);
    segment.SetData(offset++, 0x060);



    auto addressLabels = AddressLabelList();

    TestAddressLabels(addressLabels);


    Assert::Log(L"SegmentTest completed");

}

void SegmentTest::TestAddressLabels(AddressLabelList& addressLabels) {

    addressLabels.Clear();
    for (Memory::address address = 0x3000; address < 0x4000; address++) {
        // Deterministic shuffling
        auto effectiveAddress = address;
        if (address & 0x80) {
            effectiveAddress = effectiveAddress ^ 0x07f;
        }
        if (address & 0x200) {
            effectiveAddress = effectiveAddress ^ 0x133;
        }
        addressLabels.AllocateAddressLabel(effectiveAddress);
    }

    for (Memory::address address = 0x2000; address < 0x4000; address++) {
        auto addressLabel = addressLabels.FindAddressLabel(address);
        if (address < 0x3000) {
            Assert::BoolEquals(addressLabel, false);
        }
        else {
            Assert::BoolEquals(addressLabel, true);
            Assert::LongEquals(addressLabel->GetAddress(), address);
            Assert::BoolEquals(addressLabel->IsAligned(), false);
            Assert::LongEquals(addressLabel->GetNearestAddress(), 0x0000);
        }
    }

    // Ensure that the list is sorted
    Memory::address address = 0x3000;
    AddressLabelList::AddressLabelVector addressLabelsVector;
    addressLabels.Enumerate(addressLabelsVector);
    for (const auto addressLabel : addressLabelsVector) {
        Assert::LongEquals(addressLabel->GetAddress(), address++);
    }

    addressLabels.AlignAddressLabels(0x3800);

    for (address = 0x3000; address < 0x4000; address++) {
        auto addressLabel = addressLabels.FindAddressLabel(address);
        Assert::PointerNotNull((void*)addressLabel);
        if (address < 0x3800) {
            Assert::BoolEquals(addressLabel->IsAligned(), false);
            Assert::LongEquals(addressLabel->GetNearestAddress(), 0x0000);
        }
        else if (address == 0x3800) {
            Assert::BoolEquals(addressLabel->IsAligned(), true);
            Assert::LongEquals(addressLabel->GetNearestAddress(), 0x0000);
        }
        else {
            Assert::BoolEquals(addressLabel->IsAligned(), false);
            Assert::LongEquals(addressLabel->GetNearestAddress(), 0x03800);
        }
    }

}
