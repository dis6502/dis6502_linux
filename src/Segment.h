#pragma once

#include "Syntax.h"
#include "Comment.h"
#include "Fixup.h"
#include "ProcessorType.h"
#include "InstructionSet.h"
#include "MemoryBlock.h"
#include "FileHeader.h"
#include "Memory.h"
#include "MemoryType.h"
#include "SegmentTypes.h"
#include "Symbol.h"
#include "XML.h"
#include "DisassemblyResultTypes.h"
#include "AddressLabel.h"
#include "AddressLabelList.h"
#include <gsl/gsl>

class InputStream;

class Segment : public XML::Serializable {
public:
    static constexpr size_t LABEL_PREFIX_LENGTH = 20;

    typedef char TITLE[22];
    typedef char LABEL_PREFIX[LABEL_PREFIX_LENGTH];

    typedef byte SDX_BLOCK_NUMBER;

    static const wchar_t* szLabelFormat;
    static const wchar_t* szDefaultLabelFormat;
    static const wchar_t* szLabelOffsetFormat;
    static const wchar_t* szDefaultLabelOffsetFormat;

    wstring         szTitle;

    Memory::address wBegin;            // Only for ATARI_BINARY and SDX_FIXED_BLK
    Memory::address wEnd;              // End address for ATARI_BINARY and SDX_FIXED_BLK. Size of fix-ups for SDX_SYM_REQUIRED

    bool            bBinary;           // Is it a code segment to disassemble ?

    wstring         szLabelPrefix;     // Optional prefix for labels, does not contain the separator
    Memory::word	wSDXFixUpSize;     // Only for SDX_SYM_REQUIRED and SDX_FIX_UP_BLK
    SDX_BLOCK_NUMBER bSDXBlockNumber;  // SDX block number
    byte			bSDXControlByte;   // SDX control byte for SDX_RELOC_BLK
    wstring			szSDXSymbol;       // SDX symbol name for SDX_SYM_REQUIRED and SDX_SYM_DEFINED

    ProcessorType	processorType;		// Processor type

    MemoryBlock 	memoryBlock;

    CommentList     comments;	        // List of comments in this segment

    // Transient attributes which are not serialized to XML
    SymbolList symbols;                 // List of SDX system symbols to fix-up. DO NOT SAVE IN WORKSPACE
    FixupList fixups;                   // List of addresses to fix-up. DO NOT SAVE IN WORKSPACE

    Segment();
    Segment(const Segment& a) = delete;           // Copy constructor
    Segment& operator=(const Segment&) = delete;  // Copy assignment
    Segment(Segment&&) = delete;                  // Move constructor
    Segment& operator=(Segment&&) = delete;       // Move assignment
    ~Segment() override;


    void Clear();
    FileHeader GetHeader() const;
    bool IsHeader(const FileHeader wHeader) const;
    void SetHeader(FileHeader header);
    Memory::size GetSize() const;
    bool IsEmpty() const;
    bool IsSDX() const;

    wstring ToString() const;

    bool IsSplittable() const;
    bool CanSplitAt(Memory::offset offset) const;
    void SplitAt(Memory::offset offset, Segment& newSegment);

    bool IsMergeable() const;
    bool CanMergeWith(const Segment& nextSegment) const;
    void MergeWith(const Segment& nextSegment);

    bool IsSDXRelocBlkWithData() const;
    bool IsSDXRelocBlkWithoutData() const;
    const wstring_view GetSDXBlockType() const;
    const wstring_view GetSDXMemoryType() const;

    void ClearMemoryBlock();
    void CreateMemoryBlockFromBeginToEnd();
    void CreateMemoryBlockWithSize(Memory::size size);
    void CreateMemoryBlockFromFile(Memory::size size, InputStream& inputStream); // throws IOException

    byte GetData(Memory::offset offset) const;
    Memory::word GetWord(Memory::offset offset) const;
    void SetData(Memory::offset offset, byte data);
    void SetData(Memory::offset offset, const ByteSequence& data, size_t dataOffset, Memory::size dataSize);

    MemoryType GetType(Memory::offset offset) const;
    bool IsType(Memory::offset offset, MemoryType memoryType) const;
    bool IsUnknown(Memory::offset offset) const;

    void SetType(Memory::offset offset, MemoryType memoryType);
    void SetType(Memory::offset offset, MemoryType memoryType, Memory::size size);

    bool ContainsAddress(Memory::address) const;

    void ClearComments();
    gsl::not_null<Comment*> AllocateComment();
    wstring FindComment(Memory::offset offset) const;
    void DeleteComments(Memory::offset offset, Memory::size size);

    void AllocateSymbol(Memory::address address, wstring_view symbol);
    const Symbol* FindSymbol(Memory::address wAddr) const;

    void ClearFixups();
    Fixup* AllocateFixup(SEGMENT_INDEX labelSegmentIndex, gsl::not_null<Segment*> labelSegment, Memory::address wAddr);
    const Fixup* FindFixup(Memory::address wAddr) const;

    gsl::not_null<AddressLabelList*> GetFixupAddressLabels();
    gsl::not_null<AddressLabelList*> GetAddressLabels();

    // Define an address label.
    // If there is a matching fixup address label, it will be set to aligned .
    // Otherwise a matching code address label will be set to aligned.
    void DefineAddressLabel(Memory::address address);
    void AlignAddressLabels(Memory::address address);
    void AlignRamBlkAddresses();

    void Load14(InputStream& inputStream); // throws IOException
    void SerializeTo(XML::Element& element) const override;
    void DeserializeFrom(const XML::Element& element) override;

    void SetFirstLineNumber(DisassemblyResultLineNumber firstLineNumber);
    DisassemblyResultLineNumber GetFirstLineNumber() const; // 0 means it has not be set at all


private:
    static constexpr Memory::size MAX_SEGMENT_SIZE = 0x10000;

    FileHeader       wHeader;            // Magic word for file header type.

    // Transient attributes which are not serialized to XML
    AddressLabelList fixupAddressLabels; // List of address labels defined in this segments through fixup. DO NOT SAVE IN WORKSPACE
    AddressLabelList addressLabels;      // List of addresses referenced by code. DO NOT SAVE IN WORKSPACE

    DisassemblyResultLineNumber firstLineNumber; // First line in CODE_SECTION part of listing of this segment. DO NOT SAVE IN WORKSPACE

};
