#include "SegmentList.h"

#include <cassert>

#include "Application.h"
#include "Debug.h"
#include "DisassemblySectionType.h"
#include "Comment.h"
#include "FileHeader.h"
#include "Fixup.h"
#include "Equate.h"
#include "String.h"
#include "EquateList.h"
#include "InstructionSet.h"
#include "Memory.h"
#include "Segment.h"
#include "Workspace.h"
#include <gsl/util>
#ifdef _WIN32
#include <Windows.h>
#endif

#include "SegmentListInserter.h"
#include "SegmentListChangedListener.h"

SegmentList::SegmentList(Workspace* workspace) {
    this->workspace = workspace;
    selectedIndex = NO_SEGMENT_INDEX;
}

SegmentList::~SegmentList() {
    for (auto& segment : segmentList) {
        delete segment;
    }
    workspace = nullptr;
}

/*
** Clear segment list
*/
void SegmentList::Clear() {
    BeginUpdate();
    SetSelectedIndex(NO_SEGMENT_INDEX);
    for (auto& segment : segmentList) {
        segment->Clear();
    }
    segmentList.clear();
    globalSegment.Clear();
    NotifyListeners(Property::SEGMENTS);
    EndUpdate();
}

bool SegmentList::IsEmpty() const {
    return segmentList.empty();
}

/*
** Returns the number of used segments
*/
SEGMENT_COUNT SegmentList::GetCount() const {
    return gsl::narrow<SEGMENT_COUNT>(segmentList.size());
}

SEGMENT_INDEX SegmentList::GetSelectedIndex() const {
    return selectedIndex;
}

void SegmentList::SetSelectedIndex(SEGMENT_INDEX segmentIndex) {
    selectedIndex = segmentIndex;
    NotifyListeners(Property::SELECTED_INDEX);
}

std::unique_ptr<SegmentListInserter> SegmentList::CreateInserter() {
    return std::make_unique<SegmentListInserter>(*this);
}

gsl::not_null<Segment*> SegmentList::GetGlobalSegment() {
    return &globalSegment;
}

SEGMENT_INDEX SegmentList::GetSegmentIndex(const Segment* segment) const {
    if (segment == nullptr) {
        return NO_SEGMENT_INDEX;
    }

    const auto segmentCount = GetCount();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++) {
        if (segmentList.at(segmentIndex) == segment) {
            return segmentIndex;
        }
    }

    return NO_SEGMENT_INDEX;
}

gsl::not_null<Segment*> SegmentList::GetSegment(SEGMENT_INDEX segmentIndex) const {
    return segmentList.at(segmentIndex);
}

gsl::not_null<const Segment*> SegmentList::GetConstSegment(SEGMENT_INDEX segmentIndex) const {
    return segmentList.at(segmentIndex);
}

gsl::not_null<Segment*> SegmentList::AddSegment() {
    return InsertSegmentAt(GetCount());
}

gsl::not_null <Segment*> SegmentList::InsertSegmentAt(SEGMENT_INDEX segmentIndex) {
    if (segmentIndex > GetCount()) {
        throw std::runtime_error("Invalid index larger than the current size of the list"); // TODO: Use application exception
    }

    auto segment = new Segment();
    auto it = segmentList.begin();

    segmentList.insert(it + segmentIndex, segment);

    return segment;
}

void SegmentList::DeleteSegment(SEGMENT_INDEX segmentIndex) {
    delete segmentList.at(segmentIndex);

    auto it = segmentList.begin();
    segmentList.erase(it + segmentIndex);
    NotifyListeners(Property::SEGMENTS);
}
/*
** check if a fixup exists for this address
*/
const Fixup* SegmentList::GetFixup(SEGMENT_INDEX segmentIndex, MemoryType cDisByteType, Memory::address wPC, Memory::address wAddr) {
    return segmentList.at(segmentIndex)->FindFixup(wPC);
}

/*
** Return the segment whose block number is given in parameter
*/
Segment* SegmentList::FindBySDXBlockNumber(byte sdxBlockNumber) const {

    // try yo find a block with the same id
    for (auto& segment : segmentList) {
        if (segment->IsHeader(FileHeader::SDX_RELOC_BLK)) {
            if (segment->bSDXBlockNumber == sdxBlockNumber) {
                return segment;
            }
        }
    }

    // no block id match. get the block of code
    int blockNumber = 0;

    for (auto& segment : segmentList) {

        if (segment->IsHeader(FileHeader::ATARI_BINARY)
            || segment->IsHeader(FileHeader::SDX_FIXED_BLK)
            || segment->IsHeader(FileHeader::SDX_RELOC_BLK)) {
            blockNumber++;
            if (blockNumber == sdxBlockNumber) {
                return segment;
            }
        }
    }

    return nullptr;
}

/*
** return the index of the segment whose block number is given in parameter
*/
SEGMENT_INDEX SegmentList::FindByAddr(Memory::address wAddr) const {
    // try yo find a block with the same id
    const auto segmentCount = GetCount();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++) {

        const auto& segment = segmentList.at(segmentIndex);
        if ((segment->IsHeader(FileHeader::ATARI_BINARY))
            || (segment->IsHeader(FileHeader::SDX_FIXED_BLK))
            || (segment->IsHeader(FileHeader::SDX_RELOC_BLK))) {
            if ((segment->wBegin <= wAddr) && (segment->wEnd >= wAddr)) {
                return segmentIndex;
            }
        }
    }
    return NO_SEGMENT_INDEX;
}

/*
** return the index of the segment which contains the address
*/
SEGMENT_INDEX SegmentList::FindSegmentByFixedAddr(SEGMENT_INDEX segmentIndex, Memory::address address) const {

    // Try yo find it in the current segment first
    auto& segment = segmentList.at(segmentIndex);
    if (segment->ContainsAddress(address)) {
        return segmentIndex;
    }

    // Not found, try to find it in all segmentList.
    const auto segmentCount = GetCount();
    for (segmentIndex = 0; segmentIndex < segmentCount; segmentIndex++) {
        auto& segment = segmentList.at(segmentIndex);
        if ((segment->IsHeader(FileHeader::ATARI_BINARY)) || (segment->IsHeader(FileHeader::SDX_FIXED_BLK))) {
            if (segment->ContainsAddress(address)) {
                return segmentIndex;
            }
        }
    }
    return NO_SEGMENT_INDEX;
}

/*
** return the index of the segment for the target address but with the same kind of segment (file header)
*/
SEGMENT_INDEX SegmentList::FindSegmentWithSameFileHeaderByAddr(SEGMENT_INDEX segmentIndex, Memory::address address) const {

    // Try yo find it in the current segment first
    const auto& sourceSegment = segmentList.at(segmentIndex);
    if (sourceSegment->ContainsAddress(address)) {
        return segmentIndex;
    }

    // Not found, try to find it in all other segmentList.
    const auto segmentCount = GetCount();
    for (SEGMENT_INDEX otherSegmentIndex = 0; otherSegmentIndex < segmentCount; otherSegmentIndex++) {
        if (otherSegmentIndex != segmentIndex) {
            auto& otherSegment = segmentList.at(otherSegmentIndex);
            if (otherSegment->IsHeader(sourceSegment->GetHeader()) && otherSegment->ContainsAddress(address)) {
                return segmentIndex;
            }
        }
    }
    return NO_SEGMENT_INDEX;
}

SEGMENT_COUNT SegmentList::MergeSegments() {
    BeginUpdate();

    SEGMENT_COUNT mergedCount = 0;
    const auto count = GetCount();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < count - 1; segmentIndex++) {
        auto& segment = segmentList.at(segmentIndex);
        auto& nextSegment = segmentList.at(segmentIndex + 1);

        if (segment->CanMergeWith(*nextSegment)) {
            segment->MergeWith(*nextSegment);
            DeleteSegment(segmentIndex + 1);
            mergedCount++;
        }
    }
    if (mergedCount > 0) {
        SetSelectedIndex(0);
    }
    EndUpdate();
    return mergedCount;
}

void SegmentList::SplitSelectedSegment(Memory::offset offset) {
    const auto segmentIndex = GetSelectedIndex();
    if (segmentIndex == NO_SEGMENT_INDEX) {
        return;
    }
    auto& segment = segmentList.at(segmentIndex);

    BeginUpdate();

    if (!segment->memoryBlock.IsEmpty()) {
        const auto& newSegment = InsertSegmentAt(segmentIndex + 1);
        segment->SplitAt(offset, *newSegment);

        SetSelectedIndex(segmentIndex + 1);
    }
    EndUpdate();
}


void SegmentList::DeleteSelectedSegment() {
    auto segmentIndex = GetSelectedIndex();
    if (segmentIndex == NO_SEGMENT_INDEX) {
        return;
    }

    BeginUpdate();
    DeleteSegment(segmentIndex);

    // Select previous segment if there is ny
    if (segmentIndex > 0) {
        segmentIndex--;
    }
    else {
        segmentIndex = NO_SEGMENT_INDEX;
    }
    SetSelectedIndex(segmentIndex);
    EndUpdate();
}

void SegmentList::MoveSelectedSegmentUp() {
    const auto segmentIndex = GetSelectedIndex();
    if (segmentIndex > 0) {
        const auto newSegmentIndex = segmentIndex - 1;
        auto& old = segmentList.at(segmentIndex);
        segmentList.at(segmentIndex) = segmentList.at(newSegmentIndex);
        segmentList.at(newSegmentIndex) = old;
        NotifyListeners(Property::SEGMENTS);
        SetSelectedIndex(newSegmentIndex);

    }
}

void SegmentList::MoveSelectedSegmentDown() {
    const auto segmentIndex = GetSelectedIndex();
    BeginUpdate();
    if (segmentIndex < GetCount() - 1) {
        const auto newSegmentIndex = segmentIndex + 1;
        auto& old = segmentList.at(segmentIndex);
        segmentList.at(segmentIndex) = segmentList.at(newSegmentIndex);
        segmentList.at(newSegmentIndex) = old;
        NotifyListeners(Property::SEGMENTS);
        SetSelectedIndex(newSegmentIndex);
    }
    EndUpdate();
}

void SegmentList::FreeAllSymbols() {
    for (auto& segment : segmentList) {
        segment->symbols.clear();
    }
}

void SegmentList::FreeAllFixups() {
    for (auto& segment : segmentList) {
        segment->ClearFixups();
    }
}

void SegmentList::FreeAllFixupAddressLabels() {
    for (auto& segment : segmentList) {
        segment->GetFixupAddressLabels()->Clear();
    }
}

bool SegmentList::AllocateAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, const Instruction& instruction) {

    return AllocateAddress(segmentIndex, wPC, wAddr, cType, instruction.GetOperandMode(), instruction.GetLabelAccess());
}

bool SegmentList::AllocateAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess) {

    if (cType == MemoryType::SYMBOL) {
        return false;
    }
    if (workspace->GetUserEquateList()->FindEquateByAddress(wAddr, labelAccess, true)) { // TODO: Why not check system equates, too?
        return false;
    }
    if (GetFixup(segmentIndex, cType, wPC, wAddr)) {
        return false;
    }
    if (wMode != OperandMode::Relative) {
        const auto& segment = segmentList.at(segmentIndex);
        if (workspace->GetSystemEquateList()->FindEquateByAddress(wAddr, labelAccess, segment->IsSDX())) {
            return false;
        }
    }
    else {
        // For a relative branch, find in the same kind of segment first
        const auto otherSegmentIndex = FindSegmentWithSameFileHeaderByAddr(segmentIndex, wAddr);
        if (otherSegmentIndex != NO_SEGMENT_INDEX) {
            AllocateSegmentAddress(otherSegmentIndex, wAddr);
            return true;
        }
    }
    const auto otherSegmentIndex = FindSegmentByFixedAddr(segmentIndex, wAddr);
    if (otherSegmentIndex == NO_SEGMENT_INDEX) {
        globalSegment.GetAddressLabels()->AllocateAddressLabel(wAddr);
    }
    else {
        AllocateSegmentAddress(otherSegmentIndex, wAddr);
    }
    return true;
}

void SegmentList::AllocateSegmentAddress(SEGMENT_INDEX segmentIndex, Memory::address wAddr) {
    const auto& segment = GetSegment(segmentIndex);
    segment->GetAddressLabels()->AllocateAddressLabel(wAddr);
}

void SegmentList::FreeAllAddresses() {
    for (auto& segment : segmentList) {
        segment->GetAddressLabels()->Clear();
    }
    globalSegment.GetAddressLabels()->Clear();
}

void SegmentList::AlignRamBlkLabelAddresses() {
    for (auto& segment : segmentList) {
        segment->AlignRamBlkAddresses();
    }
}

wstring SegmentList::DefineLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address address, bool& defined) const {
    static wchar_t szLabelBuffer[Segment::LABEL_PREFIX_LENGTH + 50];

    defined = false;
    auto equate = workspace->GetUserEquateList()->FindAndMarkEquateByAddress(address, LabelAccess::READ);
    if (equate) {
        defined = true;
        return equate->GetLabel();
    }
    const auto& segment = segmentList.at(segmentIndex);
    const auto addressLabel = segment->GetFixupAddressLabels()->FindNearestAddressLabel(address); // TODO: This is redundant code for addresses/address labels
    bool found = false;
    if (addressLabel) {
        if (addressLabel->IsAligned()) {
            address = addressLabel->GetAddress();
        }
        else {
            address = addressLabel->GetNearestAddress();
        }
        equate = workspace->GetUserEquateList()->FindAndMarkEquateByAddress(address, LabelAccess::READ);
        if (equate) {
            defined = true;
            return equate->GetLabel();
        }
        found = true;
    }
    else {
        const auto _address = segment->GetAddressLabels()->FindNearestAddressLabel(address);
        if (_address) {
            if (_address->IsAligned()) {
                address = _address->GetAddress();
            }
            else {
                address = _address->GetNearestAddress();
            }
            equate = workspace->GetUserEquateList()->FindAndMarkEquateByAddress(address, LabelAccess::READ);
            if (equate) {
                return equate->GetLabel();
            }
            found = true;
        }
    }
    if (found) {
        if (segment->szLabelPrefix.empty() && segment->IsSDX()) {
            wsprintf(szLabelBuffer, Segment::szDefaultLabelFormat, segmentIndex, address);
        }
        else {
            wsprintf(szLabelBuffer, Segment::szLabelFormat, segment->szLabelPrefix.c_str(), address);
        }
        defined = true;
        return szLabelBuffer;
    }
    return L"";
}


wstring SegmentList::BuildAddress(SEGMENT_INDEX segmentIndex, Memory::address wAddr, LabelAccess labelAccess, bool bNoNearest) {
    auto& segment = segmentList.at(segmentIndex);
    auto addressLabel = segment->GetAddressLabels()->FindAddressLabel(wAddr);
    if (addressLabel) {
        const auto wNearestAddr = addressLabel->GetNearestAddress();
        if ((addressLabel->IsAligned()) || (wAddr == wNearestAddr) || (wNearestAddr == 0) || (bNoNearest)) {
            wAddr = addressLabel->GetAddress();
            if (segment->IsSDX()) {
                wsprintf(String::szBuffer, Segment::szDefaultLabelFormat, segmentIndex, wAddr);
            }
            else {
                wsprintf(String::szBuffer, L"L%04hX", wAddr);
            }
        }
        else {
            wAddr = addressLabel->GetAddress();
            auto equate = workspace->GetUserEquateList()->FindEquateByAddress(wNearestAddr, labelAccess, true);
            if (equate) {
                equate->AddLabelReference(labelAccess);
                wsprintf(String::szBuffer, L"%s+%hu", equate->GetLabel(), wAddr - wNearestAddr);
            }
            else {
                if (segment->IsSDX()) {
                    wsprintf(String::szBuffer, Segment::szDefaultLabelOffsetFormat, segmentIndex, wNearestAddr, wAddr - wNearestAddr);
                }
                else {
                    wsprintf(String::szBuffer, L"L%04hX+%hu", wNearestAddr, wAddr - wNearestAddr);
                }
            }
        }
        return String::Format();
    }
    addressLabel = globalSegment.GetAddressLabels()->FindAddressLabel(wAddr);
    if (addressLabel) {
        wsprintf(String::szBuffer, L"L%04hX", wAddr);
        return String::Format();;
    }
    return String::Empty();;
}

wstring SegmentList::GetLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode) {
    auto& segment = segmentList.at(segmentIndex);
    const auto& instructionSet = workspace->GetInstructionSet(segment->processorType);
    const auto& instruction = instructionSet->GetInstruction(opcode);
    return GetLabelAtAddress(segmentIndex, wPC, wAddr, cType, instruction.GetOperandMode(), instruction.GetLabelAccess());
}

wstring SegmentList::GetLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess) {
    // Private delegation method to place central breakpoints
    auto result = GetLabelAtAddressInternal(segmentIndex, wPC, wAddr, cType, wMode, labelAccess);
    return result;
}


wstring SegmentList::GetLabelAtAddressInternal(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess) {

    auto equate = workspace->GetUserEquateList()->FindEquateByAddress(wAddr, labelAccess, true);
    if (equate != nullptr) {
        equate->AddLabelReference(labelAccess);
        return equate->GetLabel();
    }

    const auto sdx = GetSegment(segmentIndex)->IsSDX();
    if ((wMode == OperandMode::ZeroPageX) || (wMode == OperandMode::ZeroPageY) || (wMode == OperandMode::ZeroPage) || (wMode == OperandMode::IndexedIndirect) || (wMode == OperandMode::IndirectIndexed)) {
        equate = workspace->GetSystemEquateList()->FindEquateByAddress(wAddr, labelAccess, sdx);
        if (equate != nullptr) {
            return equate->GetLabel();;
        }
        return BuildAddress(segmentIndex, wAddr, labelAccess, TRUE);
    }
    const Fixup* fixup = GetFixup(segmentIndex, cType, wPC, wAddr);
    if (fixup) {
        segmentIndex = fixup->GetLabelSegmentIndex();
        auto& segment = segmentList.at(segmentIndex);
        const auto addressLabel = segment->GetFixupAddressLabels()->FindAddressLabel(wAddr);
        if (addressLabel) {
            const auto wNearestAddr = addressLabel->GetNearestAddress();
            if ((addressLabel->IsAligned()) || (wAddr == wNearestAddr)) {
                wAddr = addressLabel->GetAddress();
                wsprintf(String::szBuffer, Segment::szDefaultLabelFormat, segmentIndex, wAddr);
            }
            else {
                wAddr = addressLabel->GetAddress();
                equate = workspace->GetUserEquateList()->FindEquateByAddress(wNearestAddr, labelAccess, true);
                const Memory::offset offset = wAddr - wNearestAddr;
                if (equate) {
                    equate->AddLabelReference(labelAccess);
                    wsprintf(String::szBuffer, L"%s+%hu", equate->GetLabel(), offset);
                }
                else {
                    wsprintf(String::szBuffer, Segment::szDefaultLabelOffsetFormat, segmentIndex, wNearestAddr, offset);
                }
            }
            return String::Format();
        }
        return String::Empty();
    }
    if (wMode == OperandMode::Relative) {
        // For a relative branch, find in the same kind of segment first
        const auto otherSegmentIndex = FindSegmentWithSameFileHeaderByAddr(segmentIndex, wAddr);
        if (otherSegmentIndex != NO_SEGMENT_INDEX) {
            auto label = BuildAddress(otherSegmentIndex, wAddr, labelAccess, FALSE);
            if (!label.empty()) {
                return label;
            }
        }
    }
    const auto otherSegmentIndex = FindSegmentByFixedAddr(segmentIndex, wAddr);
    if (otherSegmentIndex != NO_SEGMENT_INDEX) {
        segmentIndex = otherSegmentIndex;
    }
    auto label = BuildAddress(segmentIndex, wAddr, labelAccess, FALSE);
    if (!label.empty()) {
        return label;
    }
    equate = workspace->GetSystemEquateList()->FindEquateByAddress(wAddr, labelAccess, sdx);
    if (equate != nullptr) {
        return equate->GetLabel();
    }
    return String::Empty();
}

wstring SegmentList::GetUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size) {
    wstring result;

    const auto& segment = segmentList.at(segmentIndex);

    if (size != 0xFFFF) {
        if (size == 0) {
            size = 1;
        }

        Memory::offset relativeOffset = 0;
        while (relativeOffset < size) { // TODO: Is this correct?
            auto comment = segment->FindComment(offset + relativeOffset);

            if (!comment.empty()) {
                if (result.empty()) {
                    result = comment;
                }
                else {
                    result = result.append(L"\n").append(comment);
                }
            }

            relativeOffset++;
        }
    }

    return result;
}

void SegmentList::SetUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size, wstring_view text) {

    if (size != 0xFFFF) {
        auto& segment = segmentList.at(segmentIndex);

        segment->DeleteComments(offset, size);
        if (!text.empty()) {
            const auto& comment = segment->AllocateComment();
            comment->SetOffset(offset);
            comment->SetText(text);
        }
    }
}

void SegmentList::AddListener(SegmentListChangedListener& listener) {
    try {
        listeners.emplace_back(&listener);
    }
    catch (const std::bad_alloc&) {
        std::terminate();
    }
}

void SegmentList::RemoveListeners() {
    listeners.clear();
}

void SegmentList::BeginUpdate() {
    updateCounter++;
}

void SegmentList::EndUpdate() {
    assert(updateCounter > 0);
    updateCounter--;
    if (updateCounter == 0) {
        FlushEvents();
    }
}
void SegmentList::NotifySegmentContentChanged() {
    NotifyListeners(Property::SEGMENT_CONTENT);
}

void SegmentList::NotifyListeners(SegmentList::Property property) {
    Debug::LogValue(L"SegmentList::NotifyListeners: Property", ToString(property));

    // Add each event only once
    auto it = std::find(propertyChangeEvents.begin(), propertyChangeEvents.end(), property);
    if (it == propertyChangeEvents.end()) {
        propertyChangeEvents.push_back(property);
        if (updateCounter == 0) {
            FlushEvents();
        }
    }
}

void SegmentList::FlushEvents() {
    if (propertyChangeEvents.size() == 0) {
        return;
    }

    wstring text;
    for (auto property : propertyChangeEvents) {
        text = text + ToString(property) + L" ";
    }
    Debug::LogValue(L"SegmentList::FlushEvents: Properties", text);


    for (auto& listener : listeners) {
        listener->HandleSegmentListChanged(*this, propertyChangeEvents);
    }
    propertyChangeEvents.clear();
}

void SegmentList::SerializeTo(XML::Element& element) const {
    SetWordAttribute(Count, GetCount());

    for (auto& segment : segmentList) {
        const auto& segmentElement = XML::AddChildElement(element, L"Segment");
        segment->SerializeTo(*segmentElement);
    }
}

void SegmentList::DeserializeFrom(const XML::Element& element) {

    BeginUpdate();

    Clear();

    word localCount;
    GetWordAttribute(Count, localCount);
    const SEGMENT_COUNT count = localCount;  // Local to not have race condition with instance attribute

    if (count > 0) {
        auto segmentElement = element.FirstChildElement();

        for (SEGMENT_INDEX segmentIndex = 0; (segmentElement != nullptr) && (segmentIndex < count); segmentIndex++) {
            const auto& segment = AddSegment();
            segment->DeserializeFrom(*segmentElement);
            segmentElement = segmentElement->NextSiblingElement("Segment");
        }

        SetSelectedIndex(0);
    }
    EndUpdate();
}
