#pragma once

#include "Byte.h"
#include "Word.h"

#include "EquateType.h"
#include "LabelAccess.h"
#include "Memory.h"
#include "Syntax.h"
#include "XML.h"
class EquateList;

// One line including the comment and the zero termination
typedef wchar_t EQUATE_LINE[251];


class Equate final : public XML::Serializable {

friend EquateList;

public:
    typedef word LabelValue;
    // Extracts the 4 character hexadecimal address from labels name Lnnnn, "" otherwise
    static wstring ExtractAddress(wstring_view label); // TODO: For what? AutomaticLabel, MainDisassembly and ....?
    static bool IsAutomaticLabel(wstring_view label);
    static bool IsLabelWithOffset(wstring_view label);

    EquateType GetType() const;

    const wchar_t* GetLabel() const;  // Intentionally const wchar_t* for performance
    bool EqualsLabel(wstring_view label) const;

    const wchar_t* GetBaseLabel() const;  // Intentionally const wchar_t* for performance
    bool IsRange() const;

    inline LabelAccess GetLabelAccess() const;
    inline bool IsLabelAccessSupported(LabelAccess labelAccess) const;
    inline LabelValue GetLabelValue() const;
    const wchar_t* GetComment() const; // Intentionally const wchar_t* for performance

    void ClearDefinition();
    bool HasDefinition() const;
    void AddDefinition();

    void ClearReferences();
    void AddLabelReference(LabelAccess labelAccess);
    LabelAccess GetReferencedLabelAccess() const;
    bool HasReferences() const;
    inline bool HasReferencedLabelAccess(LabelAccess labelAccess) const;

    static void ReadFrom(wstring_view line, EquateType& equateType, wstring& label, LabelAccess& labelAccess, Memory::address& address, wstring& comment, wstringstream& error);

    // Serialization.
    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;

    wstring ToString() const;

private:

    EquateType equateType;
    wstring label;
    LabelAccess labelAccess; // Supported types of access
    LabelValue labelValue;
    wstring comment;

    wstring baseLabel; // Transient
    bool defined; // Transient
    LabelAccess referencedLabelAccess; // Transient, referenced types of access

    // Bitmask utility method.
    inline static bool IsLabelAccessSupported(LabelAccess supportedLabelAccess, LabelAccess labelAccess);

    // Returns true and index = string.size(), if the string starting at index only contains spaces.
    static bool SkipBlanks(wstring_view string, size_t& index);

    // Private methodss.
    void Clear();
    void InitTransientFields();

    // Private methods used by friend classes.
    Equate();
    void Init(EquateType equateType, wstring_view label, LabelAccess labelAccess, LabelValue labelValue, wstring_view comment);

};

bool Equate::IsLabelAccessSupported(LabelAccess supportedLabelAccess, LabelAccess labelAccess) {
    const auto a = (byte)supportedLabelAccess;
    const auto b = (byte)labelAccess;
    return ((a & b) != 0);
}

LabelAccess Equate::GetLabelAccess() const {
    return labelAccess;
}

Equate::LabelValue Equate::GetLabelValue() const {
    return labelValue;
}


bool Equate::IsLabelAccessSupported(LabelAccess labelAccess) const {
    return IsLabelAccessSupported(this->labelAccess, labelAccess);
}

bool Equate::HasReferencedLabelAccess(LabelAccess labelAccess) const {
    return IsLabelAccessSupported(this->referencedLabelAccess, labelAccess);
}

