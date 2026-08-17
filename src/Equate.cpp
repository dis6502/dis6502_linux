#include "Character.h"
#include "Debug.h"
#include "Equate.h"
#include "Strings.h"
#ifdef _WIN32
#include <Windows.h>
#endif

wstring Equate::ExtractAddress(wstring_view label) {
    if (label.length() < 5) {
        return L"";
    }
    int start = label.length() - 5;
    if (label.at(start++) != 'L') {
        return L"";
    }
    for (int i = start; i < start + 4; i++) {
        if (((label.at(i) < '0') || (label.at(i) > '9')) && ((label.at(i) < 'A') || (label.at(i) > 'F'))) {
            return L"";
        }
    }
    return wstring(label.substr(start));
}

bool Equate::IsAutomaticLabel(wstring_view label) {
    return !ExtractAddress(label).empty();
}

bool Equate::IsLabelWithOffset(wstring_view label) {
    return label.find_first_of(L"+-") != wstring::npos;
}

Equate::Equate() {
    Clear();
}

void Equate::Clear() {
    equateType = EquateType::UNKNOWN;
    label.clear();
    labelAccess = LabelAccess::UNKNOWN;
    labelValue = 0;
    comment.clear();

    InitTransientFields();
}


void Equate::InitTransientFields() {

    // Transient information
    switch (equateType) {
    case EquateType::UNKNOWN:
    case EquateType::EMPTY:
    case EquateType::COMMENT:
        if (!label.empty()) {
            throw std::runtime_error("Label specified");
        }
        break;

    case EquateType::LABEL: {

        if (labelAccess == LabelAccess::UNKNOWN) {
            throw std::runtime_error("Invalid access");
        }

        if (label.empty()) {
            throw std::runtime_error("No label specified");
        }

        auto index = label.find('+');
        if (index == string_view::npos) {
            index = label.find('-');
        }
        if (index != string_view::npos) {
            this->baseLabel = label.substr(0, index);
        }
        break;
    }

    default:
        throw std::runtime_error("Invalid equate type");

    };

    defined = false;
    referencedLabelAccess = LabelAccess::UNKNOWN;
}

void Equate::Init(EquateType equateType, wstring_view label, LabelAccess labelAccess, LabelValue labelValue, wstring_view comment) {

    this->equateType = equateType;
    this->label = label;
    this->labelAccess = labelAccess;
    this->labelValue = labelValue;
    this->comment = comment;

    InitTransientFields();
}

EquateType Equate::GetType() const {
    return equateType;
}

const wchar_t* Equate::GetLabel() const {
    return label.c_str();
}

bool Equate::EqualsLabel(wstring_view label) const {
    return this->label.compare(label) == 0;
}


const wchar_t* Equate::GetBaseLabel() const {
    return baseLabel.c_str();
}

bool Equate::IsRange() const {
    return !baseLabel.empty();
}

const wchar_t* Equate::GetComment() const {
    return comment.c_str();
}

bool Equate::HasDefinition() const {
    return defined;
}

void Equate::ClearDefinition() {
    defined = false;
}


void Equate::AddDefinition() {
    defined = true;
}



void Equate::ClearReferences() {
    referencedLabelAccess = LabelAccess::UNKNOWN;
}

void Equate::AddLabelReference(LabelAccess labelAccess) {
    referencedLabelAccess = (LabelAccess)((byte)referencedLabelAccess | (byte)labelAccess);
    // TODO: Debug::Log(wstring(L"Reference of access type ") + GetAccessString(access) + L" added for " + ToString());
}

LabelAccess Equate::GetReferencedLabelAccess() const {
    return referencedLabelAccess;
}

bool Equate::HasReferences() const {
    return referencedLabelAccess != LabelAccess::UNKNOWN;
}

bool Equate::SkipBlanks(wstring_view string, size_t& index) {
    while (index < string.size() && Character::IsSpace(string.at(index))) {
        index++;
    }
    return index == string.size();
}

void Equate::ReadFrom(wstring_view line, EquateType& equateType, wstring& label, LabelAccess& labelAccess, Memory::address& address, wstring& comment, wstringstream& error) {

    equateType = EquateType::UNKNOWN;
    address = 0;
    labelAccess = LabelAccess::UNKNOWN;
    label.clear();
    comment.clear();
    error.clear();

    size_t index = 0;
    if (SkipBlanks(line, index)) {
        equateType = EquateType::EMPTY;
        return;
    }

    /*
    ** If we have a comment line, we add it to the disassembly listing.
    */
    if (line.at(index) == ';') {
        equateType = EquateType::COMMENT;

        /*
        ** skip blanks
        */
        index++;
        if (!SkipBlanks(line, index)) {
            comment = String::Trim(line.substr(index));
        }
        return;
    }

    /*
    ** We must have a label name starting with a letter or "_".
    */
    auto c = line.at(index);
    if (!Character::IsAlpha(c) || c == '_') {
        error << L"Character '" << c << "' at position " << index << " is not a valid start character for a label name.";
        return;
    }
    equateType = EquateType::LABEL;

    /*
    ** Compose the label name. Label names can contain offsets,e g. "RTCLOK+1 = $13"
    */
    while ((index < line.size()) && (Character::IsAlphaNumeric(c) || (c == '_') || (c == '+') || (c == '-'))) {
        label.push_back(c);
        index++;
        if (index < line.size()) {
            c = line.at(index);
        }
    }

    if (SkipBlanks(line, index)) {
        error << L"No access qualifier specified.";
        return;
    }

    c = line.at(index);
    /*
    ** Now we must have an equal (=, <, > or #) sign.
    */
    switch (c) {

    case '=':
        labelAccess = LabelAccess::READ_WRITE;
        break;
    case '<':
        labelAccess = LabelAccess::READ;
        break;
    case '>':
        labelAccess = LabelAccess::WRITE;
        break;
    case '#':
        labelAccess = LabelAccess::IMMEDIATE;
        break;

    default:
        error << L"Character '" << c << "' at position " << index + 1 << " is not an access qualifier. Use '=', '<', '>' or '#'.";
        return;
    }
    index++;

    /*
    ** Skip blanks.
    */
    if (SkipBlanks(line, index)) {
        error << L"No value specified.";
        return;
    }


    /*
    ** Now we must have an address (hex or decicmal).
    */
    size_t count = 0;
    if (line.at(index) == '$') {
        index++;
        const auto p = line.data() + index;
        if (swscanf(p, L"%hx%n", &address, &count) == 0) {
            error << L"Characters '" << wstring(p) << "' at position " << index + 1 << " cannot be interpreted as a hexadecimal number.";
            return;
        }
    }
    else {
        const auto p = line.data() + index;
        if (swscanf(p, L"%hu%n", &address, &count) == 0) {
            error << L"Characters '" << wstring(p) << "' at position " << index + 1 << " cannot be interpreted as a decimal number.";
            return;;
        }
    }
    index += count;

    /*
    ** Skip blanks.
    */
    if (SkipBlanks(line, index)) {
        return;
    }

    /*
    ** Scan for line comment.
    */
    c = line.at(index);
    if (c == ';') {

        index++;
        if (!SkipBlanks(line, index)) {
            comment = String::Trim(line.substr(index));
        }
        return;
    }

    error << L"Invalid character '" << c << "' after value found. Line end or comment expected";
}
void Equate::SerializeTo(XML::Element& element) const {

    SetStdStringAttribute("EquateType", EquateTypeFactory::GetInfo(equateType).key);
    switch (equateType) {
    case EquateType::UNKNOWN:
        throw std::runtime_error("Invalid equate type");
    case EquateType::EMPTY:
        break;
    case EquateType::COMMENT:
        SetStdStringAttribute(Comment, comment);
        break;
    case EquateType::LABEL:
        SetStdStringAttribute(Label, label);
        SetStdStringAttribute(LabelAccess, LabelAccessFactory::GetInfo(labelAccess).key);
        if (labelValue < 0x100) {
            SetByteAttributeHex(LabelValue, (byte)labelValue);
        } else {
            SetWordAttributeHex(LabelValue, labelValue);
        }
        if (!comment.empty()) {
            SetStdStringAttribute(Comment, comment);
        }

    }
}

void Equate::DeserializeFrom(const XML::Element& element) {
    Clear();
    wstring equateTypeString;
    GetStdStringAttribute(EquateType, equateTypeString);
    equateType = EquateTypeFactory::GetInfo(equateTypeString).equateType;
    GetStdStringAttribute(Label, label);
    wstring labelAccesstring;
    GetStdStringAttribute(LabelAccess, labelAccesstring);
    labelAccess = LabelAccessFactory::GetInfo(labelAccesstring).labelAccess;
    GetWordAttribute(LabelValue, labelValue);
    GetStdStringAttribute(Comment, comment);
    InitTransientFields();
}

wstring Equate::ToString() const {
    switch (equateType) {
    case EquateType::EMPTY:
        break;

    case EquateType::COMMENT:
        wsprintf(String::szBuffer, L"; %s", GetComment());
        break;

    case EquateType::LABEL:
        if (!comment.empty()) {
            wsprintf(String::szBuffer, L"%s %s $%04hX; %s", GetLabel(), LabelAccessFactory::GetQualifier(labelAccess), GetLabelValue(), GetComment());
        }
        else {
            wsprintf(String::szBuffer, L"%s %s $%04hX", GetLabel(), LabelAccessFactory::GetQualifier(labelAccess), GetLabelValue());
        }
        break;

    default:
        throw std::runtime_error("Unsupported equate type");
        break;
    }
    return String::Format();
}
