#pragma once

#include "DisassemblySectionType.h" // TODO: Separate EquateListType from DisassemblySectionType
#include "Equate.h"
#include "EquateListChangedListener.h"
#include "WorkspaceTypes.h"

#include <vector>
#include "XML.h"

class Window;
class InputStream;
class OutputStream;
class Workspace;

class EquateList final : public XML::Serializable {
public:

    EquateList(WorkspaceProperty property); // TODO: Should rather be an EquateListType
    ~EquateList();

    const std::vector<gsl::not_null<Equate*>>& GetEquates() const;

    void Clear();
    bool IsEmpty() const;
    size_t GetCount() const;
    size_t GetLabelCount() const;

    WorkspaceProperty GetProperty() const;

    Equate* FindEquateByAddress(Memory::address address, LabelAccess labelAccess, bool sdx) const;
    const Equate* FindAndMarkEquateByAddress(Memory::address address, LabelAccess labelAccess) const;

    Equate* GetEquateByLabel(wstring_view label);

    // Parse an equate from a line in a definition file. Returns nullptr and a message in the application log if the line cannot be parsed.
    Equate* AddEquate(wstring_view line);
    void SetRange(wstring_view label, Memory::address labelAddress, Memory::address startAddress, Memory::address endAddress);

    // Serialization called from Workspace1.
    void Load1X(InputStream& inputStream); // throws IOException
    void Save1X(OutputStream& outputStream) const; // throws IOException

    // Serialization called from Workspace.
    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;

    // Called from EquateListLogic.
    bool Load(wstring_view filePath); // throws IOException
    void Save(wstring_view filePath, bool xasm) const;

    void AddListener(EquateListChangedListener& listener);
    void RemoveListeners();

    // Methods for transient attributes.
    void ClearFlags();
    void SetBaseLabelsReferenced();
    bool IsEquateAddressReferenced(Memory::address address, LabelAccess labelAccess) const;

private:
    WorkspaceProperty property;
    std::vector<gsl::not_null<Equate*>> equateList;

    // Event handling
    std::vector<gsl::not_null<EquateListChangedListener*>> listeners;
    void NotifyListeners();

    gsl::not_null<Equate*> AddEquate(); // Does not fire events. 

    void AddRange(wstring_view label, Memory::address wLabelAddr, Memory::address wStartAddr, Memory::address wEndAddr);
    void RemoveRange(Memory::address wStartAddr, Memory::address wEndAddr);
};