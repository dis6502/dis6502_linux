#include "Debug.h"
#include "Segment.h"
#include "Workspace.h"
#include "DatatypeUtility.h"
#include "MemoryInspectorSelection.h"


MemoryInspectorSelection::MemoryInspectorSelection(Workspace& workspace) {

    this->workspace = &workspace;
    Clear();
}


void MemoryInspectorSelection::Clear() {
    segmentIndex = NO_SEGMENT_INDEX;
    segment = nullptr;
    ClearSelection();
}

SEGMENT_INDEX MemoryInspectorSelection::GetSegmentIndex() const {
    return segmentIndex;
}

bool MemoryInspectorSelection::HasSegment() const {
    return segmentIndex != NO_SEGMENT_INDEX;
}

void MemoryInspectorSelection::SetSegmentIndex(SEGMENT_INDEX segmentIndex) {
    this->segmentIndex = segmentIndex;
    if (this->segmentIndex == NO_SEGMENT_INDEX) {
        segment = nullptr;
    }
    else {
        segment = workspace->GetSegmentList()->GetSegment(this->segmentIndex);
    }
}

const Segment* MemoryInspectorSelection::GetConstSegment() const {
    return segment;
}

void MemoryInspectorSelection::ClearSelection() {
    bSelection = false;
    nBegin = 0;
    nEnd = 0;
    Debug::Log(L"MemoryInspectorSelection::ClearSelection()");
}

bool MemoryInspectorSelection::HasSelection() const {
    return HasSegment() && !segment->IsEmpty() && bSelection;
}

bool MemoryInspectorSelection::IsEmpty() const {
    return !HasSelection();
}

void  MemoryInspectorSelection::SetSelection(Memory::offset nBegin, Memory::offset nEnd) {
//    if (nBegin < 0) {
//      throw std::runtime_error("Cannot set negative begin of range.");
//  }

//  if (nEnd < 0) {
//      throw std::runtime_error("Cannot set negative end of range.");
//  }

    if (segment == nullptr) {
        throw std::runtime_error("No segment selected yet. Cannot set selection range.");
    }

    const auto size = segment->GetSize();

    if (nBegin >= size) {
        nBegin = size - 1;
    }

    if (nEnd >= size) {
        nEnd = size - 1;
    }

    if (nBegin <= nEnd) {
        this->nBegin = nBegin;
        this->nEnd = nEnd;
    }
    else {
        this->nBegin = nEnd;
        this->nEnd = nBegin;
    }

    this->bSelection = true;
    Debug::LogValue(L"MemoryInspectorSelection::SetSelection", DatatypeUtility::WordToHexString(this->nBegin, true) + L" - " + DatatypeUtility::WordToHexString(this->nEnd, true));

}

Memory::offset MemoryInspectorSelection::GetBegin() const {
    return nBegin;
}

Memory::offset MemoryInspectorSelection::GetEnd() const {
    return nEnd;
}

Memory::size MemoryInspectorSelection::GetSize() const {
    if (IsEmpty()) {
        return 0;
    }

    return GetEnd() - GetBegin() + 1;
}

bool MemoryInspectorSelection::GetAddressRage(Memory::address& wBegin, Memory::address& wEnd) const {
    if (segment != nullptr) {
        wBegin = segment->wBegin + nBegin;
        wEnd = segment->wBegin + nEnd;

        return true;
    }

    return false;
}


std::unique_ptr<ByteSequence> MemoryInspectorSelection::GetByteSequence() const {
    if (HasSelection()) {
        return segment->memoryBlock.GetData()->GetSubSequence(GetBegin(), GetSize());
    }
    return std::make_unique<ByteArray>();
}
