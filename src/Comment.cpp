#include "Comment.h"
#include "Strings.h"

Comment::Comment() :
    offset(0),
    text(L"") {
}

Comment::~Comment() {
}

void Comment::Clear() {
    offset = 0;
    SetText(L"");
}

Memory::offset Comment::GetOffset() const {
    return offset;
}

void Comment::SetOffset(Memory::offset value) {
    offset = value;
}

wstring Comment::GetText() const {
    return text;
}

void Comment::SetText(wstring_view value) {
    // Trim trailing whitespaces.
    text = String::Trim(value);
}

void Comment::SerializeTo(XML::Element& element) const {
    SetWordAttributeHex(Offset, offset);
    SetStdStringAttribute(Text, text);
}

void Comment::DeserializeFrom(const XML::Element& element) {
    GetWordAttribute(Offset, offset);
    GetStdStringAttribute(Text, text);
}
