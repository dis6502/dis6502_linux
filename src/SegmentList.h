#pragma once

#include "Syntax.h"

#include <vector>

#include "Equate.h"
#include "SegmentTypes.h"
#include "Memory.h"
#include "XML.h"
#include "Segment.h"
#include "InstructionSet.h"

class SegmentListInserter;
class SegmentListChangedListener;
enum class MemoryType : byte;
enum class OperandMode;
class Workspace;

class SegmentList : public XML::Serializable {
public:
    static constexpr SEGMENT_COUNT MAX_SEGMENTS = 4096;

    enum class Property {
        SEGMENTS,
        SEGMENT_CONTENT,
        SELECTED_INDEX,
    };

    SegmentList() = delete;
    SegmentList(Workspace* workspace);                   // Workspace can be nullptr, is not equate handling is performed later
    SegmentList(const SegmentList& a) = delete;          // copy constructor
    SegmentList& operator=(const SegmentList&) = delete; // copy assignment
    SegmentList(SegmentList&&) = delete;                 // move constructor
    SegmentList& operator=(SegmentList&&) = delete;      // move assignment
    ~SegmentList();

    void Clear();
    bool IsEmpty() const;
    SEGMENT_COUNT GetCount() const;
    SEGMENT_INDEX GetSelectedIndex() const;
    void SetSelectedIndex(SEGMENT_INDEX segmentIndex);

    std::unique_ptr<SegmentListInserter> CreateInserter();
    gsl::not_null<Segment*> GetGlobalSegment();
    SEGMENT_INDEX GetSegmentIndex(const Segment* segment) const;
    gsl::not_null<Segment*> GetSegment(SEGMENT_INDEX segmentIndex) const;
    gsl::not_null<const Segment*> GetConstSegment(SEGMENT_INDEX segmentIndex) const;

    Segment* FindBySDXBlockNumber(byte sdxBlockNumber) const;
    SEGMENT_INDEX FindByAddr(Memory::address wAddr) const;


    gsl::not_null<Segment*> InsertSegmentAt(SEGMENT_INDEX segmentIndex);

    SEGMENT_COUNT MergeSegments();
    void DeleteSelectedSegment();
    void SplitSelectedSegment(Memory::offset wOffset);
    void MoveSelectedSegmentUp();
    void MoveSelectedSegmentDown();

    void FreeAllSymbols();
    void FreeAllFixups();
    void FreeAllFixupAddressLabels();

    bool AllocateAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, const Instruction& instruction);
    bool AllocateAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess);
    void FreeAllAddresses();

    void AlignRamBlkLabelAddresses();
    wstring DefineLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address wAddr, bool& defined) const;

    wstring GetLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, byte opcode);
    wstring GetLabelAtAddress(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess);

    wstring GetUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size);
    void SetUserComment(SEGMENT_INDEX segmentIndex, Memory::offset offset, Memory::size size, wstring_view text);

    void AddListener(SegmentListChangedListener& listener);
    void RemoveListeners();
    void BeginUpdate();
    void EndUpdate();
    void NotifySegmentContentChanged();

    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;

private:
    friend class SegmentListInserter;

    Workspace* workspace;
    std::vector<gsl::not_null<Segment*>> segmentList;
    Segment globalSegment;

    SEGMENT_INDEX selectedIndex;

    SEGMENT_INDEX FindSegmentByFixedAddr(SEGMENT_INDEX segmentIndex, Memory::address wAddr) const;
    SEGMENT_INDEX FindSegmentWithSameFileHeaderByAddr(SEGMENT_INDEX segmentIndex, Memory::address wAddr) const;
    void AllocateSegmentAddress(SEGMENT_INDEX segmentIndex, Memory::address wAddr);

    const Fixup* GetFixup(SEGMENT_INDEX segmentIndex, MemoryType cDisByteType, Memory::address wPC, Memory::address wAddr);
    wstring BuildAddress(SEGMENT_INDEX segmentIndex, Memory::address wAddr, LabelAccess labelAccess, bool bNoNearest);
    wstring GetLabelAtAddressInternal(SEGMENT_INDEX segmentIndex, Memory::address wPC, Memory::address wAddr, MemoryType cType, OperandMode wMode, LabelAccess labelAccess);

    // Event handling
    int updateCounter = 0;
    std::vector<Property> propertyChangeEvents;
    std::vector<gsl::not_null<SegmentListChangedListener*>> listeners;

    void NotifyListeners(Property property);
    void FlushEvents();

    gsl::not_null<Segment*> AddSegment(); // Does not fire events. Returns nullptr if list is full and sends error message to applicaton
    void DeleteSegment(SEGMENT_INDEX segmentIndex);
};

constexpr const wchar_t* ToString(SegmentList::Property value) {
    switch (value) {
    case SegmentList::Property::SEGMENTS:
        return L"SEGMENTS";
    case SegmentList::Property::SEGMENT_CONTENT:
        return L"SEGMENT_CONTENT";
    case SegmentList::Property::SELECTED_INDEX:
        return L"SELECTED_INDEX";
    }
    return L"";
}
