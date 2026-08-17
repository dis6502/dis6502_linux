#include "AddressLabelList.h"


void AddressLabelList::Enumerate(AddressLabelVector& addressLabelsVector) const {
    addressLabelsVector.clear();
    for (const auto& addressLabel : addressLabels) {
        addressLabelsVector.push_back(addressLabel.get());
    }
}

void AddressLabelList::Clear() {
    addressLabels.clear();
    addressLabelsMap.clear();

}

void AddressLabelList::AllocateAddressLabel(Memory::address address) {


    if (FindAddressLabel(address)) {
        return; // Already allocated
    }
    auto it = addressLabels.begin();
    if (it != addressLabels.end()) {
        const gsl::not_null<AddressLabel*> firstEntry = it->get();
        if ((address < firstEntry->GetAddress())) {  // Smaller than first entry?
            auto it2 = addressLabels.insert(addressLabels.begin(), std::make_unique<AddressLabel>(address));
            addressLabelsMap[address] = it2->get();
            return;
        }
        it++;

        while (it != addressLabels.end() && (it->get()->GetAddress() < address)) {
            it++;
        }
    }

    auto it2 = addressLabels.insert(it, std::make_unique<AddressLabel>(address));
    addressLabelsMap[address] = it2->get();
}

const AddressLabel* AddressLabelList::FindAddressLabel(Memory::address address) const {
    return FindMutableAddressLabel(address);
}

AddressLabel* AddressLabelList::FindMutableAddressLabel(Memory::address address) const {
    auto it = addressLabelsMap.find(address);
    if (it == addressLabelsMap.end()) {
        return nullptr;
    }
    return it->second;
}

void AddressLabelList::AlignAddressLabels(Memory::address address) {
    for (auto const& addressLabel : addressLabels) {

        if (!addressLabel->IsAligned()) {
            if (addressLabel->GetAddress() == address) {
                addressLabel->SetAligned(true);
            }
            else if ((addressLabel->GetAddress() > address) && (addressLabel->GetNearestAddress() < address)) {
                addressLabel->SetNearestAddress(address);
            }
        }
    }
}

void AddressLabelList::AlignNearestAddress() {
    for (auto const& addressLabel : addressLabels) {
        addressLabel->SetNearestAddress(addressLabel->GetAddress());
    }
}

const AddressLabel* AddressLabelList::FindNearestAddressLabel(Memory::address address) const {
    auto result = FindAddressLabel(address);
    if (result != nullptr && result->IsAligned()) {
        return result;
    }

    for (auto const& addressLabel : addressLabels) {
        if (addressLabel->GetNearestAddress() == address) {
            return addressLabel.get();
        }
    }

    return nullptr;
}