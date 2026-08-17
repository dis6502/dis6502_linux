#pragma once

#include "Syntax.h"
#include "Memory.h"
#include "AddressLabel.h"

#include <vector>
#include <map>
#include <gsl/gsl>

class AddressLabelList {

public:
    typedef std::vector<gsl::not_null<AddressLabel*>> AddressLabelVector;

    void Enumerate(AddressLabelVector& addressLabelsVector) const;
 
    void Clear();
    void AllocateAddressLabel(Memory::address address); 
    const AddressLabel* FindAddressLabel(Memory::address address) const;

    // For all not yet aligned labels:
    // - set to "aligned", if their label address matches the specified address
    // - set the nearest address to address, if the specified address is between the label address and the current nearest address 
    void AlignAddressLabels(Memory::address address);
    void AlignNearestAddress(); // Sets nearestAddress = address
    const AddressLabel* FindNearestAddressLabel(Memory::address address) const;
    AddressLabel* FindMutableAddressLabel(Memory::address address) const;

private:
    std::vector<std::unique_ptr<AddressLabel>> addressLabels;
    std::map<Memory::address,AddressLabel*> addressLabelsMap;

};