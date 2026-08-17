#pragma once

class AddressLabelList;
class AddressList;

class SegmentTest {
public:

    static void TestSegment();

private:

    static void TestAddressLabels(AddressLabelList& addressLabels);
    static void TestAddresses(AddressList& addresses);

};