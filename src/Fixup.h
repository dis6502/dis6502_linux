#pragma once

#include "Syntax.h"
#include "Memory.h"
#include "XML.h"

#include <vector>
#include <gsl/gsl>

enum class FixupType : Memory::byte {
	ADD_250_BYTES = 0xFF,
	SET_BLOCK_NUM = 0xFE,
	SET_BLOCK_ADDR = 0xFD,
	END = 0xFC
};


class Fixup: public XML::Serializable {
public:
	Fixup();


	Memory::address GetAddress() const;
	void SetAddress(Memory::address address);
	int GetLabelSegmentIndex() const;
	void SetLabelSegmentIndex(int labelSegmentIndex);

    static bool IsAddressLess(const Fixup* a, const Fixup* b);

    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;


private:
	Memory::address address;    // address of the word to fix-up in the segment
	int labelSegmentIndex;     // segment where this address is located
};

typedef std::vector<gsl::not_null<Fixup*>> FixupList;
