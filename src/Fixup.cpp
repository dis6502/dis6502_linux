#include "Fixup.h"


Fixup::Fixup() :
	address(0),
	labelSegmentIndex(0)
{
}

Memory::address Fixup::GetAddress() const {
	return address;
}

void Fixup::SetAddress(Memory::address address) {
	this->address = address;
}

int Fixup::GetLabelSegmentIndex() const {
	return labelSegmentIndex;
}

void Fixup::SetLabelSegmentIndex(int labelSegmentIndex) {
	this->labelSegmentIndex = labelSegmentIndex;
}

bool Fixup::IsAddressLess(const Fixup* a, const Fixup* b) {
    return (a->GetAddress() < b->GetAddress());
}

void Fixup::SerializeTo(XML::Element& element) const {
    SetWordAttributeHex(Address, address);
    SetIntAttribute(LabelSegmentIndex, labelSegmentIndex);
}

void Fixup::DeserializeFrom(const XML::Element& element) {
    GetWordAttribute(Address, address);
    GetIntAttribute(LabelSegmentIndex, labelSegmentIndex);
}