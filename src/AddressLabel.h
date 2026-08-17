#pragma once

#include "Syntax.h"
#include "Memory.h"
#include "XML.h"

#include <vector>
#include <gsl/gsl>

class AddressLabel: public XML::Serializable {
public:
    AddressLabel(Memory::address address);

    inline Memory::address GetAddress() const;
    inline Memory::address GetNearestAddress() const;
    inline void SetNearestAddress(Memory::address value);
    inline bool IsAligned() const;
    inline void SetAligned(bool value);

    static bool IsAddressLess(gsl::not_null<const AddressLabel*> a, gsl::not_null<const AddressLabel*> b);

    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;
private:
    Memory::address address;        // address of the label
    Memory::address nearestAddress; // nearest address before address
    bool aligned;                   // true if the address is at the begining of an instruction
};

Memory::address AddressLabel::GetAddress() const {
    return address;
}

Memory::address AddressLabel::GetNearestAddress() const {
    return nearestAddress;
}

void AddressLabel::SetNearestAddress(Memory::address value) {
    nearestAddress = value;
}

bool AddressLabel::IsAligned() const {
    return aligned;
}

void AddressLabel::SetAligned(bool value) {
    aligned = value;
}