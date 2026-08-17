#pragma once

#include "Memory.h"
#include "Syntax.h"
#include "XML.h"
#include <vector>

class Symbol : public XML::Serializable {
public:

    Symbol(Memory::address address, wstring_view symbol);

    Memory::address GetAddress() const;
    wstring GetSymbol() const;

    static bool IsAddressLess(const Symbol* a, const Symbol* b);

    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;

private:
    Memory::address address;    // address of the word to fix-up in the segment
    wstring symbol;              // symbol name to fix-up
};

typedef std::vector<std::unique_ptr<Symbol>> SymbolList;
