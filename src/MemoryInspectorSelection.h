#pragma once

#include "Byte.h"
#include "ByteSequence.h"
#include "Memory.h"
#include "SegmentTypes.h"
#include "Syntax.h"

class Workspace;

class MemoryInspectorSelection {
public:
    Segment* segment;

    MemoryInspectorSelection(Workspace& workspace);

    void Clear();
    SEGMENT_INDEX GetSegmentIndex() const;
    bool HasSegment() const;
    void SetSegmentIndex(SEGMENT_INDEX segmentIndex);
    const Segment* GetConstSegment() const;

    void ClearSelection();
    bool HasSelection() const;
    bool IsEmpty() const;
    void SetSelection(Memory::offset nBegin, Memory::offset nEnd);
    Memory::offset GetBegin() const;
    Memory::offset GetEnd() const;
    Memory::size GetSize() const;

    bool GetAddressRage(Memory::address& wBegin, Memory::address& wEnd) const;
    std::unique_ptr<ByteSequence> GetByteSequence() const;

private:
    Workspace* workspace;

    SEGMENT_INDEX segmentIndex;

    bool bSelection;
    Memory::offset nBegin;
    Memory::offset nEnd;
};
