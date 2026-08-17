#include "AddressLabel.h"

AddressLabel::AddressLabel(Memory::address address) :
    address(address),
    nearestAddress(0),
    aligned(false)
{
}

bool AddressLabel::IsAddressLess(gsl::not_null<const AddressLabel*> a, gsl::not_null<const AddressLabel*> b) {
    return (a->GetAddress() < b->GetAddress());
}

void AddressLabel::SerializeTo(XML::Element& element) const {
    SetWordAttributeHex(Address, address);
    SetWordAttributeHex(NearestAddress, nearestAddress);
    SetBoolAttribute(Aligned, aligned);

}

void AddressLabel::DeserializeFrom(const XML::Element& element) {
    GetWordAttribute(Address, address);
    GetWordAttribute(NearestAddress, nearestAddress);
    GetBoolAttribute(Aligned, aligned);
}