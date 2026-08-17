#include "Debug.h"
#include "EquateList.h"
#include "InstructionSet.h"
#include "Profile.h"
#include "Segment.h"
#include "SegmentList.h"
#include "systems/ComputerSystem.h"
#include "systems/ComputerSystemFactory.h"
#include "systems/ComputerSystemType.h"
#include "Workspace.h"
#include "WorkspaceChangedListener.h"

Workspace::Workspace(const ComputerSystemFactory& computerSystemFactory) :
    computerSystemFactory(&computerSystemFactory),
    computerSystem(this->computerSystemFactory->GetComputerSystem(ComputerSystemType::UNKNOWN)) {

    instructionSetMOS6502 = std::make_unique<InstructionSetMOS6502>(L"MOS 6502");
    instructionSetMOS65C02 = std::make_unique<InstructionSetMOS65C02>(L"MOS 65C02");
    profile = std::make_unique<Profile>();
    systemEquateList = std::make_unique<EquateList>(WorkspaceProperty::SYSTEM_EQUATES);
    systemEquateList->AddListener(*this);
    userEquateList = std::make_unique<EquateList>(WorkspaceProperty::USER_EQUATES);
    userEquateList->AddListener(*this);
    segmentList = std::make_unique<SegmentList>(this);
    disassemblyResult = std::make_unique<DisassemblyResult>();

    Init();
    segmentList->AddListener(*this);
}

Workspace::~Workspace() {
    RemoveListeners();
    systemEquateList->RemoveListeners();
    userEquateList->RemoveListeners();
    segmentList->RemoveListeners();
    Init();
}

void Workspace::Init() {
    BeginUpdate();
    SetFilePath(L"");
    segmentList->Clear();
    systemEquateList->Clear();
    userEquateList->Clear();
    profile->Clear();

    viewDisplayAsScreenCode = false;
    viewNoDisassembly = false;
    viewDoubleHeight = true;
    EndUpdate();
}

gsl::not_null<const ComputerSystemFactory*> Workspace::GetComputerSystemFactory() const {
    return computerSystemFactory;
}

bool Workspace::IsViewDisplayAsScreenCode() const {
    return viewDisplayAsScreenCode;
}

void Workspace::SetViewDisplayAsScreenCode(bool value) {
    viewDisplayAsScreenCode = value;
}

bool Workspace::isViewNoDisassembly() const {
    return viewNoDisassembly;
}

void Workspace::SetViewNoDisassembly(bool value) {
    viewNoDisassembly = value;
}

bool Workspace::IsViewDoubleHeight() const {
    return viewDoubleHeight;
}

void Workspace::SetViewDoubleHeight(bool value) {
    viewDoubleHeight = value;
}

wstring Workspace::GetFilePath() const {
    return filePath;
}

void Workspace::SetFilePath(wstring_view filePath) {
    this->filePath = filePath;
    NotifyListeners(WorkspaceProperty::FILE_PATH);
}

void Workspace::SetComputerSystemTypeID(wstring_view id) {

    auto computerSystemType = computerSystemFactory->GetComputerSystemType(id);
    if (computerSystemType == ComputerSystemType::UNKNOWN) {
        computerSystemType = ComputerSystemType::ATARI800;
    }
    SetComputerSystemType(computerSystemType);
}

void Workspace::SetComputerSystemType(ComputerSystemType computerSystemType) {

    if (computerSystem == nullptr || computerSystem->GetType() != computerSystemType) {
        computerSystem = computerSystemFactory->GetComputerSystem(computerSystemType);
        NotifyListeners(WorkspaceProperty::COMPUTER_SYSTEM_TYPE);
        NotifyListeners(WorkspaceProperty::FONT);
    }
}

gsl::not_null<const ComputerSystem*> Workspace::GetComputerSystem() const {
    return computerSystem;
}

const gsl::not_null<InstructionSet*> Workspace::GetInstructionSet(ProcessorType processorType) const {
    switch (processorType) {
    case ProcessorType::MOS6502:
        return instructionSetMOS6502.get();
    case ProcessorType::MOS65C02:
        return instructionSetMOS65C02.get();
    default:
        throw std::runtime_error("Invalid processor type");
    }
}

HFONT Workspace::GetResizedFont() const {
    return GetComputerSystem()->GetFont(viewDoubleHeight);
}

int Workspace::GetResizedFontWidth() const {
    return 8;
}

int Workspace::GetResizedFontHeight() const {
    return viewDoubleHeight ? 16 : 8;
}

Profile* Workspace::GetProfile() const {
    return profile.get();
}

const Profile* Workspace::GetConstProfile() const {
    return profile.get();
}

EquateList* Workspace::GetSystemEquateList() {
    return systemEquateList.get();
}

const EquateList* Workspace::GetConstUserEquateList() const {
    return userEquateList.get();
}

EquateList* Workspace::GetUserEquateList() {
    return userEquateList.get();
}


const Equate* Workspace::FindEquateByAddress(Memory::address address, LabelAccess labelAccess) {

    /*
    ** Address of 0 is not allowed as label. It is the NO_LABEL value!
    */
    if (address == 0) {
        return nullptr;
    }

    auto equate = userEquateList->FindEquateByAddress(address, labelAccess, true);
    if (equate == nullptr) {
        equate = systemEquateList->FindEquateByAddress(address, labelAccess, true);
    }
    return equate;
}

const Equate* Workspace::GetEquateByLabel(const wchar_t* szLabel) {
    auto equate = userEquateList->GetEquateByLabel(szLabel);
    if (equate == nullptr) {
        equate = systemEquateList->GetEquateByLabel(szLabel);
    }
    return equate;
}

const wchar_t* Workspace::FindSymbolByAddress(SEGMENT_INDEX segmentIndex, Memory::address address, Memory::address_offset address_offset) const {
    constexpr size_t SZ_BUF_LENGTH = dis_k::SDX_SYMBOL_LEN + 1 + 5 + 1; // "L1234567+32767"
    static wchar_t szBuf[SZ_BUF_LENGTH];
    auto symbol = GetConstSegment(segmentIndex)->FindSymbol(address);

    if (symbol != nullptr) {
        wcsncpy(szBuf, symbol->GetSymbol().c_str(), SZ_BUF_LENGTH);
        wcsncat(szBuf, Memory::address_offset_to_string(address_offset).c_str(), SZ_BUF_LENGTH);
        return szBuf;
    }

    return nullptr;
}

/*
** Set flag for all labels.
*/
void Workspace::ClearEquateFlags() {
    systemEquateList->ClearFlags();
    userEquateList->ClearFlags();
}

gsl::not_null<SegmentList*> Workspace::GetSegmentList() {
    return segmentList.get();
}

gsl::not_null<const SegmentList*> Workspace::GetConstSegmentList() const {
    return segmentList.get();
}


gsl::not_null<const Segment*> Workspace::GetConstSegment(SEGMENT_INDEX segmentIndex) const {
    return segmentList->GetConstSegment(segmentIndex);
}

void Workspace::NotifyFontChanged() {
    NotifyListeners(WorkspaceProperty::FONT);
}

void Workspace::NotifyProfileChanged() {
    NotifyListeners(WorkspaceProperty::PROFILE);
}

void Workspace::NotifySelectedMemoryRangeChanged() {
    NotifyListeners(WorkspaceProperty::SELECTED_MEMORY_RANGE);
}


void Workspace::AddListener(WorkspaceChangedListener& listener) {
    listeners.push_back(&listener);
}

void Workspace::RemoveListeners() {
    listeners.clear();
}

void Workspace::BeginUpdate() {
    updateCounter++;
}

void Workspace::EndUpdate() {
    updateCounter--;
    if (updateCounter == 0) {
        FlushEvents();
    }
}

void Workspace::NotifyListeners(WorkspaceProperty property) {
    Debug::LogValue(L"Workspace::NotifyListeners: Property", ToString(property));

    // Add each event only once
    auto it = std::find(propertyChangeEvents.begin(), propertyChangeEvents.end(), property);
    if (it == propertyChangeEvents.end()) {
        propertyChangeEvents.push_back(property);
        if (updateCounter == 0) {
            FlushEvents();
        }
    }
}

void Workspace::FlushEvents() {
    if (propertyChangeEvents.size() == 0) {
        return;
    }

    wstring text;
    for (auto property : propertyChangeEvents) {
        text = text + ToString(property) + L" ";
    }
    Debug::LogValue(L"Workspace::FlushEvents: Properties", text);

    // Create local copy of the events to ensure recursion works
    // The assumption is that the listeners do not change during the recursion.
    const std::vector<WorkspaceProperty> propertyChangeEventsCopy = propertyChangeEvents;
    propertyChangeEvents.clear();
    for (auto& listener : listeners) {
        listener->HandleWorkspaceChanged(*this, propertyChangeEventsCopy);
    }
}

void Workspace::HandleEquateListChanged(const EquateList& equateList, const WorkspaceProperty workspaceProperty) {
    NotifyListeners(workspaceProperty);
}

void Workspace::HandleSegmentListChanged(const SegmentList& segmentList, const std::vector<SegmentList::Property>& propertyChangeEvents) {
    BeginUpdate();
    for (auto property : propertyChangeEvents) {
        switch (property) {
        case SegmentList::Property::SEGMENTS:
        case SegmentList::Property::SEGMENT_CONTENT:
            NotifyListeners(WorkspaceProperty::SEGMENTS);
            break;
        case SegmentList::Property::SELECTED_INDEX:
            NotifyListeners(WorkspaceProperty::SELECTED_SEGMENT);
            break;
        }
    }
    EndUpdate();
}

void Workspace::SerializeTo(XML::Element& element) const {

    const auto& computerSystemTypeID = computerSystem->GetTypeInfo()->id;
    SetStdStringAttribute(ComputerSystemTypeID, computerSystemTypeID);

    const auto& profileElement = XML::AddChildElement(element, L"Profile");
    profile->SerializeTo(*profileElement);

    const auto& systemEquatesElement = XML::AddChildElement(element, L"SystemEquates");
    systemEquateList->SerializeTo(*systemEquatesElement);

    const auto& userEquatesElement = XML::AddChildElement(element, L"UserEquates");
    userEquateList->SerializeTo(*userEquatesElement);

    const auto& segmentsListElement = XML::AddChildElement(element, L"Segments");
    segmentList->SerializeTo(*segmentsListElement);
}

void Workspace::DeserializeFrom(const XML::Element& element) {
    Init();

    wstring szComputerSystemTypeID;
    GetStdStringAttribute(ComputerSystemTypeID, szComputerSystemTypeID);
    SetComputerSystemTypeID(szComputerSystemTypeID);

    auto profileElement = element.FirstChildElement("Profile");
    if (profileElement != nullptr) {
        profile->DeserializeFrom(*profileElement);
        NotifyProfileChanged();
    }

    auto systemEquatesElement = element.FirstChildElement("SystemEquates");
    if (systemEquatesElement != nullptr) {
        systemEquateList->DeserializeFrom(*systemEquatesElement); // This will notify listeners.
    }

    auto userEquatesElement = element.FirstChildElement("UserEquates");
    if (userEquatesElement != nullptr) {
        userEquateList->DeserializeFrom(*userEquatesElement); // This will notify listeners.
    }

    auto segmentsListElement = element.FirstChildElement("Segments");
    if (segmentsListElement != nullptr) {
        segmentList->DeserializeFrom(*segmentsListElement);
    }
}

DisassemblyResult* Workspace::GetDisassemblyResult() {
    return disassemblyResult.get();
}

const DisassemblyResult* Workspace::GetConstDisassemblyResult() const {
    return disassemblyResult.get();
}
