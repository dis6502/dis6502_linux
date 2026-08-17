#include "Application.h"
#include "ByteArray.h"
#include "DatatypeUtility.h"
#include "EquateList.h"
#include "EquateType.h"
#include "FileIO.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "String.h"
#include "Workspace1X.h"

// Constants from workspace version 1X
constexpr size_t MAX_BUF_LABEL_1X = 60000;
constexpr size_t LABEL_MEM_SIZE_1X = MAX_BUF_LABEL_1X + 32;

extern std::unique_ptr<Application> g_Application;

EquateList::EquateList(WorkspaceProperty property) {
    this->property = property;
}

EquateList::~EquateList() {
    Clear();
}

WorkspaceProperty EquateList::GetProperty() const {
    return property;
}

const std::vector<gsl::not_null<Equate*>>& EquateList::GetEquates() const {
    return equateList;
}


void EquateList::AddListener(EquateListChangedListener& listener) {
    try {
        listeners.emplace_back(&listener);
    }
    catch (const std::bad_alloc&) {
        std::terminate();
    }
}

void EquateList::RemoveListeners() {
    listeners.clear();
}

void EquateList::NotifyListeners() {
    for (auto& listener : listeners) {
        listener->HandleEquateListChanged(*this, property);
    }
}


void EquateList::Clear() {
    for (auto& equate : equateList) {
        delete equate;
    }
    equateList.clear();
    NotifyListeners();
}

bool EquateList::IsEmpty() const {
    return equateList.empty();
}

size_t EquateList::GetCount() const {
    return equateList.size();
}

size_t EquateList::GetLabelCount() const {
    size_t result = 0;
    for (const auto& equate : equateList) {
        if (equate->GetType() == EquateType::LABEL) {
            result++;
        }
    }
    return result;
}

gsl::not_null<Equate*> EquateList::AddEquate() {

    auto equate = new Equate();
    equateList.push_back(equate);
    return equate;
}

Equate* EquateList::FindEquateByAddress(Memory::address address, LabelAccess access, bool sdx) const {

    // Address of 0 is not allowed as label.It is the NO_LABEL value !
    if (address == 0) {
        return nullptr;
    }

    /*
    ** Do not use SDX page 7 for non SDX segments.
    ** TODO FIXME: We must mark the system equates in a certain way instead of hardcoding page 7 here but right now it does the job.
    */
    if ((!sdx) && (address >= 0x0700) && (address <= 0x07FF)) {
        return nullptr;
    }

    for (auto& equate : equateList) {
        if ((equate->GetLabelValue() == address) && equate->IsLabelAccessSupported(access)) {
            equate->AddLabelReference(access);
            return equate;
        }
    }
    return nullptr;
}

const Equate* EquateList::FindAndMarkEquateByAddress(Memory::address address, LabelAccess access) const {
    auto equate = FindEquateByAddress(address, access, true);
    if (equate) {
        equate->AddDefinition();
    }
    return equate;
}


Equate* EquateList::GetEquateByLabel(wstring_view label) {
    for (auto& equate : equateList) {
        if (equate->EqualsLabel(label)) {
            return equate;
        }
    }
    return nullptr;
}

void EquateList::SetRange(wstring_view label, Memory::address labelAddress, Memory::address startAddress, Memory::address endAddress) {
    RemoveRange(startAddress, endAddress);
    AddRange(label, labelAddress, startAddress, endAddress);
    NotifyListeners();
}

// TODO: Add labelAccess

void EquateList::AddRange(wstring_view label, Memory::address labelAddress, Memory::address startAddress, Memory::address endAddress) {

    for (auto address = startAddress; address <= endAddress; address++) {
        wsprintf(String::szBuffer, L"%s%s$%04hX", wstring(label).c_str(), (address > labelAddress ? "+" : "-"), abs(address - labelAddress));
        auto equate = AddEquate();
        equate->Init(EquateType::LABEL, String::Format(), LabelAccess::READ_WRITE, address, L"");
    }
}

void EquateList::RemoveRange(Memory::address wStartAddr, Memory::address wEndAddr) {

    for (auto i = equateList.begin(); i != equateList.end(); i++) {
        const auto labelAddress = (*i)->GetLabelValue();
        if ((labelAddress >= wStartAddr) && (labelAddress <= wEndAddr)) {
            equateList.erase(i);
        }
    }
}

/*
** Set flag for all labels
*/
void EquateList::ClearFlags() {
    for (auto& equate : equateList) {
        equate->ClearDefinition();
        equate->ClearReferences();
    }
}
/*
** Detect labels of the formed EXAMPLE+$xxxx. When found, label EXAMPLE is marked as referenced
* TODO Consider parent equate list. Also consider recursion.
*/
void EquateList::SetBaseLabelsReferenced() {
    for (const auto& equate : equateList) {
        auto referencecdAccess = equate->GetReferencedLabelAccess();
        if (equate->IsRange() && referencecdAccess != LabelAccess::UNKNOWN) {
            auto baseEquate = GetEquateByLabel(equate->GetBaseLabel());
            // Be tolerant wrt. inconsistent label definitions.
            if (baseEquate != nullptr) {
                baseEquate->AddLabelReference(referencecdAccess);
            }
        }
    }
}

/*
** Check if a label is referenced with a certain type of access.
*/
bool EquateList::IsEquateAddressReferenced(Memory::address address, LabelAccess labelAccess) const {

    if (address == 0) {
        return false;
    }

    for (auto& equate : equateList) {

        if (equate->GetLabelValue() == address && equate->HasReferencedLabelAccess(labelAccess)) {
            return true;
        }
    }
    return false;
}

/*
** Load labels from an input stream in workspace version 1X format
*/
void EquateList::Load1X(InputStream& inputStream) {
    Workspace1X::POINTER_32BIT equateArray32;

    Clear();

    // The pointer was written as a marker. If nullptr, there are no user labels.
    inputStream.Read(&equateArray32, sizeof(equateArray32));

    if (!Workspace1X::IsNull(equateArray32)) {
        byte* equateArray;
        ByteArray equateArrayBuffer(LABEL_MEM_SIZE_1X);
        equateArray = equateArrayBuffer.get();
        DatatypeUtility::ClearByteArray(equateArray, LABEL_MEM_SIZE_1X);
        try {
            inputStream.Read(equateArray, LABEL_MEM_SIZE_1X);
        }
        catch (const std::exception& ex) {
            throw ex;
        }

        auto p = equateArray;

        while (p < equateArray + MAX_BUF_LABEL_1X) {
            const auto labelAddress = Memory::to_address(*p, *(p + 1));
            if (labelAddress == 0) {
                break; // Unused equate slot
            }
            p = p + 3;
            const LabelAccess access = (LabelAccess)(*p);
            p = p + 1;
            const char* szLabel = (const char*)p; // ##char OK
            p = p + strlen(szLabel) + 1;

            auto equate = AddEquate();
            equate->Init(EquateType::LABEL, String::ansi_to_wstring(szLabel), access, labelAddress, L"");
        }
    }
}

/*
** Save user labels to an output stream in workspace version 1X format
*/
void EquateList::Save1X(OutputStream& outputStream) const {
    Workspace1X::POINTER_32BIT equateArray32;
    if (IsEmpty()) {
        equateArray32 = (Workspace1X::POINTER_32BIT)0;
    }
    else {
        equateArray32 = (Workspace1X::POINTER_32BIT)1; // Symbolic value
    }

    // The pointer is written as a marker. If nullptr, there are no user labels.
    outputStream.Write(&equateArray32, sizeof(equateArray32));

    if (!Workspace1X::IsNull(equateArray32)) {

        byte* equateArray;
        ByteArray byteArray(LABEL_MEM_SIZE_1X);
        equateArray = byteArray.get();
        DatatypeUtility::ClearByteArray(equateArray, LABEL_MEM_SIZE_1X);
        byte* p = equateArray;
        for (auto& equate : equateList) {
            if (p < equateArray + MAX_BUF_LABEL_1X) {
                (*(Memory::address*)p) = equate->GetLabelValue();
                p = p + 3;
                (*p) = (Memory::byte)equate->GetLabelAccess();
                p = p + 1;
                auto szLabel = String::wstring_to_ansi(equate->GetLabel()); // Workspace1X
                strncpy((char*)p, szLabel.c_str(), szLabel.size()); // ##char OK
                p = p + szLabel.length() + 1;
            }
            else {
                break; // To not write access labels
            }
        }
        outputStream.Write(equateArray, LABEL_MEM_SIZE_1X);
    }
}


void EquateList::SerializeTo(XML::Element& element) const {
    SetWordAttribute(Count, GetCount());

    for (auto& equate : equateList) {
        const auto& equateElement = XML::AddChildElement(element, L"Equate");
        equate->SerializeTo(*equateElement);
    }
}

void EquateList::DeserializeFrom(const XML::Element& element) {

    //BeginUpdate();

    Clear();

    word count;
    GetWordAttribute(Count, count);

    if (count > 0) {
        auto equateElement = element.FirstChildElement();

        for (size_t equateIndex = 0; (equateElement != nullptr) && (equateIndex < count); equateIndex++) {
            const auto& equate = AddEquate();
            equate->DeserializeFrom(element);
            equateElement = equateElement->NextSiblingElement("Equate");
        }
    }
    //EndUpdate();
}

Equate* EquateList::AddEquate(wstring_view line) {
    EquateType equateType;
    wstring label;
    LabelAccess labelAccess;
    Memory::address address;
    wstring comment;
    wstringstream error;

    Equate::ReadFrom(line, equateType, label, labelAccess, address, comment, error);


    auto errorString = error.str();
    if (!errorString.empty()) {
        // ERROR: Cannot parse equate line '{0}'. Error: {1}
        g_Application->SendErrorMessageWithID(IDS_ERR_CANNOT_PARSE_EQUATE_LINE, line, errorString);
        return nullptr;
    }

    if (equateType == EquateType::UNKNOWN) {
        return nullptr;
    }
    auto equate = AddEquate();
    equate->Init(equateType, label, labelAccess, address, comment);
    return equate;
}


bool EquateList::Load(wstring_view filePath) {

    Clear();

    /*
    ** Open file for reading.
    */
    g_Application->SendInfoMessageWithID(IDS_LOG_OPEN_EQUATE_FILE, filePath);

    auto lines = FileIO::ReadStrings(filePath);
    for (auto line : lines) {
        AddEquate(line);
    }

    // INFO: {0} equate lines with {1} labels loaded.
    g_Application->SendInfoMessageWithID(IDS_EQUATE_LIST_LOADED, std::to_wstring(GetCount()), std::to_wstring(GetLabelCount()));

    NotifyListeners();
    return true;
}

void EquateList::Save(wstring_view filePath, bool xasm) const {

    g_Application->SendInfoMessageWithID(IDS_LOG_SAVE_EQUATE_FILE, filePath);

    OutputStream* outputStream = nullptr;

    try {

        if (!xasm) {
            outputStream = OutputStream::OpenFile(filePath, Encoding::UTF8);
            for (const auto& equate : equateList) {
                outputStream->WriteString(equate->ToString());
            }
        }
        else {
            wstringstream os;
            for (const auto& equate : equateList) {
                os << L"        " << DatatypeUtility::WordToHexString(equate->GetLabelValue(), false) << L" " << equate->GetLabel() << "\n";
            }
            outputStream = OutputStream::OpenFile(filePath, Encoding::ASCII);
            outputStream->WriteString(L"xasm 3.0.0\nLabel table:\n");
            outputStream->WriteString(os.str());
        }
        delete outputStream;
    }
    catch (const IOException& ex) {
        g_Application->SendErrorMessageWithException(ex);

    }
}


