#include "Symbol.h"


Symbol::Symbol(Memory::address address, wstring_view symbol) :
    address(address), symbol(symbol) {

}

Memory::address Symbol::GetAddress() const {
    return address;
}

wstring Symbol::GetSymbol() const {
    return symbol;
}

bool Symbol::IsAddressLess(const Symbol* a, const Symbol* b){
    return (a->GetAddress() < b->GetAddress());
}

void Symbol::SerializeTo(XML::Element& element) const {
    SetWordAttributeHex(Address, address);
    SetStdStringAttribute(Symbol, symbol);
}

void Symbol::DeserializeFrom(const XML::Element& element) {
    GetWordAttribute(Address, address);
    GetStdStringAttribute(Symbol, symbol);
}