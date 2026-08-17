#pragma once

#include "DisassemblyResult.h"
#include "Equate.h"
#include "EquateList.h"
#include "EquateListChangedListener.h"
#include "LabelAccess.h"
#include "Memory.h"
#include "Profile.h"
#include "SegmentList.h"
#include "SegmentListChangedListener.h"
#include "SegmentTypes.h"
#include "Syntax.h"
#include "PlatformCompat.h" // For portable HFONT stand-in
#include "WorkspaceTypes.h"
#include "XML.h"
#include <gsl/gsl>
#include <vector>

enum class ComputerSystemType;
class ComputerSystemFactory;
class ComputerSystem;
class InstructionSet;
enum class ProcessorType;
class WorkspaceChangedListener;


class Workspace final : public XML::Serializable, private EquateListChangedListener, private SegmentListChangedListener {
public:

    enum class Format {
        WORKSPACE14,
        WORKSPACE36
    };

    Workspace(const ComputerSystemFactory& aComputerSystemFactory);
    ~Workspace();

    Workspace(const Workspace& a) = delete;           // no copy constructor
    Workspace& operator=(const Workspace&) = delete;  // no copy assignment
    Workspace(Workspace&&) = delete;                  // no move constructor
    Workspace& operator=(Workspace&&) = delete;       // no move assignment

    void Init();
    gsl::not_null<const ComputerSystemFactory*> GetComputerSystemFactory() const;

    bool IsViewDisplayAsScreenCode() const;
    void SetViewDisplayAsScreenCode(bool value);
    bool isViewNoDisassembly() const;
    void SetViewNoDisassembly(bool value);
    bool IsViewDoubleHeight() const;
    void SetViewDoubleHeight(bool value);

    // File.
    wstring GetFilePath() const;
    void SetFilePath(wstring_view filePath);

    // Font.
    HFONT GetResizedFont() const;
    int GetResizedFontWidth() const;
    int GetResizedFontHeight() const;

    // Computer System.
    void SetComputerSystemTypeID(wstring_view id);
    void SetComputerSystemType(ComputerSystemType computerSytemType);
    gsl::not_null<const ComputerSystem*> GetComputerSystem() const;

    // Computer System: InstructionSet.
    const gsl::not_null<InstructionSet*> GetInstructionSet(ProcessorType processorType) const;

    // Profile.
    Profile* GetProfile() const;
    const Profile* GetConstProfile() const;

    // Equates.
    EquateList* GetSystemEquateList();
    const EquateList* GetConstUserEquateList() const;
    EquateList* GetUserEquateList();

    // Memory Segments.
    gsl::not_null<SegmentList*> GetSegmentList();
    gsl::not_null<const SegmentList*> GetConstSegmentList() const;
    gsl::not_null<const Segment*> GetConstSegment(SEGMENT_INDEX segmentIndex) const;

    // Notification.
    void NotifyFontChanged();
    void NotifyProfileChanged();
    void NotifySelectedMemoryRangeChanged();

    void AddListener(WorkspaceChangedListener& listener);
    void RemoveListeners();
    void BeginUpdate();
    void EndUpdate();

    // Serialization.
    void SerializeTo(XML::Element& lpElement) const override;
    void DeserializeFrom(const XML::Element& lpElement) override;

    // Find equates in user equate list and then system equate list.
    const Equate* FindEquateByAddress(Memory::address address, LabelAccess labelAccess);
    const Equate* GetEquateByLabel(const wchar_t* szLabel);
    // Find the SDX symbol for this PC address.
    const wchar_t* FindSymbolByAddress(SEGMENT_INDEX segmentIndex, Memory::address address, Memory::address_offset address_offset) const;

    void ClearEquateFlags(); // Does not fire a Notify...Changed event.

    // Disassembly result, transient and not saved as part of the workspace itself.
    DisassemblyResult* GetDisassemblyResult();
    const DisassemblyResult* GetConstDisassemblyResult() const;
private:

    // Constructor arguments
    gsl::not_null<const ComputerSystemFactory*> computerSystemFactory;

    wstring filePath{};

    bool viewDisplayAsScreenCode = false;  // MemoryInspector uses internal character set (ANTIC)
    bool viewNoDisassembly = false;        // No disassembly launched if byte type is changed
    bool viewDoubleHeight = false;         // Double the font height of the display

    gsl::not_null<const ComputerSystem*> computerSystem;
    std::unique_ptr<InstructionSet> instructionSetMOS6502 = nullptr;
    std::unique_ptr<InstructionSet> instructionSetMOS65C02 = nullptr;
    std::unique_ptr<Profile> profile = nullptr;

    std::unique_ptr<EquateList> systemEquateList = nullptr;
    std::unique_ptr<EquateList> userEquateList = nullptr;
    std::unique_ptr<SegmentList> segmentList = nullptr;;
    std::unique_ptr<DisassemblyResult> disassemblyResult = nullptr;;

    // Event handling
    int updateCounter = 0;
    std::vector<WorkspaceProperty> propertyChangeEvents;
    std::vector<gsl::not_null<WorkspaceChangedListener*>> listeners;

    void NotifyListeners(WorkspaceProperty property);
    void FlushEvents();
    void HandleEquateListChanged(const EquateList& equateList, const WorkspaceProperty workspaceProperty) override;
    void HandleSegmentListChanged(const SegmentList& segmentList, const std::vector<SegmentList::Property>&) override;

};

constexpr const wchar_t* ToString(WorkspaceProperty value) {
    switch (value) {
    case WorkspaceProperty::COMPUTER_SYSTEM_TYPE:
        return L"COMPUTER_SYSTEM_TYPE";
    case WorkspaceProperty::FILE_PATH:
        return L"FILE_PATH";
    case WorkspaceProperty::FONT:
        return L"FONT";
    case WorkspaceProperty::PROFILE:
        return L"PROFILE";
    case WorkspaceProperty::SEGMENTS:
        return L"SEGMENTS";
    case WorkspaceProperty::SELECTED_SEGMENT:
        return L"SELECTED_SEGMENT";
    case WorkspaceProperty::SELECTED_MEMORY_RANGE:
        return L"SELECTED_MEMORY_RANGE";
    case WorkspaceProperty::SYSTEM_EQUATES:
        return L"SYSTEM_EQUATES";
    case WorkspaceProperty::USER_EQUATES:
        return L"USER_EQUATES";
    }
    return L"";
}