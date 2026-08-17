#include "AddressLabel.h"
#include "Comment.h"
#include "FileHeader.h"
#include "FileInputStream.h"
#include "Fixup.h"
#include "Segment.h"
#include "Strings.h"
#include "Symbol.h"
// #include <algorithm> // TODO For sorting, should be in each list
#ifdef _WIN32
#include <Windows.h>
#endif

// TODO: Move all formats to central place
const wchar_t* Segment::szLabelFormat = L"%sL%04hX";
const wchar_t* Segment::szDefaultLabelFormat = L"S%03hXL%04hX";
const wchar_t* Segment::szLabelOffsetFormat = L"%sL%04hX+%hu";
const wchar_t* Segment::szDefaultLabelOffsetFormat = L"S%03hXL%04hX+%hu";


Segment::Segment() {
    Clear();
}

Segment::~Segment() {
    Clear();
}

void Segment::Clear() {
    szTitle.clear();

    wHeader = FileHeader::RAW;
    wBegin = 0;
    wEnd = 0;

    bBinary = false;
    processorType = ProcessorType::MOS6502;
    szLabelPrefix.clear();
    wSDXFixUpSize = 0;
    bSDXBlockNumber = 0;
    bSDXControlByte = 0;
    szSDXSymbol.clear();

    ClearMemoryBlock();
    ClearComments();
    symbols.clear();
    ClearFixups();
    fixupAddressLabels.Clear();
    addressLabels.Clear();

    firstLineNumber = 0;
}

FileHeader Segment::GetHeader() const {
    return wHeader;
}

bool Segment::IsHeader(const FileHeader wHeader) const {
    return (this->wHeader == wHeader);
}

void Segment::SetHeader(FileHeader header) {
    this->wHeader = header;
}


Memory::size Segment::GetSize() const {
    if (memoryBlock.IsEmpty()) {
        return 0;
    }

    return wEnd - wBegin + 1;
}

bool Segment::IsEmpty() const {
    return memoryBlock.IsEmpty();
}

bool Segment::IsSDX() const {
    return (wHeader == FileHeader::SDX_FIXED_BLK) || (wHeader == FileHeader::SDX_SYM_REQUIRED) || (wHeader == FileHeader::SDX_SYM_DEFINED)
        || (wHeader == FileHeader::SDX_FIX_UP_BLK) || (wHeader == FileHeader::SDX_RELOC_BLK);
}

wstring Segment::ToString() const {
    wstring symbol = L"";
    const auto size = GetSize();
    auto szBuffer = String::szBuffer;

    // Read the header depending on the type.
    switch (wHeader) {
    case FileHeader::SDX_RELOC_BLK:
        wsprintf(szBuffer, L"%s blk %hu $%04hX-$%04hX len %hu mem $%02hX", wstring(GetSDXBlockType()).c_str(), (unsigned int)bSDXBlockNumber, wBegin, wEnd, size, (Memory::word)bSDXControlByte);
        break;

    case FileHeader::SDX_FIX_UP_BLK:
        wsprintf(szBuffer, L"Fixups blk %hu len %hu", (unsigned int)bSDXBlockNumber, size);
        break;

    case FileHeader::SDX_SYM_REQUIRED:
        // trim trailing whitespaces
        symbol = String::Trim(szSDXSymbol);
        wsprintf(szBuffer, L"SymReq fixup len %-4hu %s", size, symbol.c_str());
        break;

    case FileHeader::SDX_SYM_DEFINED:
        // trim trailing whitespaces
        symbol = String::Trim(szSDXSymbol);
        wsprintf(szBuffer, L"SymDef blk %hu ofs $%04hX %s", (unsigned int)bSDXBlockNumber, wBegin, symbol.c_str());
        break;

    case FileHeader::ATARI_BINARY:
        wsprintf(szBuffer, L"StdBin %04hX-$%04hX len %04hX (%hu)", wBegin, wEnd, size, size);
        break;

    case FileHeader::SDX_FIXED_BLK:
        wsprintf(szBuffer, L"StdBlk $%04hX-$%04hX len $%04hX (%hu)", wBegin, wEnd, size, size);
        break;

    default:
        if (bBinary) {
            wsprintf(szBuffer, L"Binary $%04hX-$%04hX len $%04hX (%hu)", wBegin, wEnd, size, size);
        }
        else {
            wsprintf(szBuffer, L"Raw    $%04hX-$%04hX len $%04hX (%hu)", wBegin, wEnd, size, size);
        }
        break;
    }

    return String::Format();
}

bool Segment::IsSplittable() const {
    return (IsHeader(FileHeader::ATARI_BINARY) || IsHeader(FileHeader::SDX_FIXED_BLK)) && !IsEmpty();
}

bool Segment::CanSplitAt(Memory::offset offset) const {
    return IsSplittable() && offset > 0 && offset < GetSize();
}

void Segment::SplitAt(Memory::offset offset, Segment& newSegment) {

    if (!CanSplitAt(offset)) {
        throw std::runtime_error("Invalid offset");
    }
    newSegment.wHeader = this->wHeader;
    newSegment.wBegin = this->wBegin + offset;
    newSegment.wEnd = this->wEnd;
    newSegment.CreateMemoryBlockFromBeginToEnd();

    this->memoryBlock.CopyTo(offset, newSegment.GetSize(), newSegment.memoryBlock, 0);
    this->wEnd = this->wBegin + offset - 1;

    for (auto it = comments.begin(); it != comments.end();) {
        auto const& comment = *it;
        const auto currentOffset = comment->GetOffset();

        // If the comment offset is equal or larger then the split offset 
        if (currentOffset >= offset) {
            // Remove it from the current segment
            it = comments.erase(it);

            // Aligned it to the new segment begin
            comment->SetOffset(currentOffset - offset);

            // Add it to the new segment
            newSegment.comments.push_back(comment);
        }
        else {
            it++;
        }
    }
}

bool Segment::IsMergeable() const {
    return IsSplittable();
}

bool Segment::CanMergeWith(const Segment& nextSegment) const {
    if (IsMergeable() && nextSegment.IsMergeable()
        && (wEnd + 1 == nextSegment.wBegin)) {
        const Memory::size newSize = GetSize() + nextSegment.GetSize();

        // Do not create segments with more than 64k
        if (newSize < MAX_SEGMENT_SIZE) {
            return true;
        }
    }

    return false;
}

void Segment::MergeWith(const Segment& nextSegment) {
    const auto size = this->GetSize();
    const Memory::size totalSize = size + nextSegment.GetSize();

    if (totalSize > MAX_SEGMENT_SIZE) {
        throw std::runtime_error("Merged segments must not exceed 64kb");
    }

    MemoryBlock memoryBlock;
    memoryBlock.Create(totalSize);

    this->memoryBlock.CopyTo(0, size, memoryBlock, 0);
    this->memoryBlock.CopyTo(0, nextSegment.GetSize(), memoryBlock, size);

    CreateMemoryBlockWithSize(totalSize);
    memoryBlock.CopyTo(0, totalSize, this->memoryBlock, 0);

    this->wEnd = nextSegment.wEnd;

    for (auto const& comment : nextSegment.comments) {
        auto const& newComment = this->AllocateComment();
        newComment->SetOffset(size + comment->GetOffset());
        newComment->SetText(comment->GetText());
    }

    // TODO: Transfer additional data?
    //ClearSymbols();
    //ClearFixups();
    //ClearLabels();
    //ClearAddresses();
}

bool Segment::IsSDXRelocBlkWithData() const {
    bool result = IsHeader(FileHeader::SDX_RELOC_BLK);
    result = result && ((bSDXControlByte & 0x80) == 0x00);

    return result;
}

bool Segment::IsSDXRelocBlkWithoutData() const {
    bool bResult = IsHeader(FileHeader::SDX_RELOC_BLK);
    bResult = bResult && ((bSDXControlByte & 0x80) == 0x80);

    return bResult;
}

const wstring_view Segment::GetSDXBlockType() const {
    const wchar_t* szResult;

    switch (bSDXControlByte & 0x080) {
    case 0x00:
        szResult = L"RelBlk";
        break;

    case 0x80:
        szResult = L"RamBlk";
        break;

    default:
        szResult = L"";
        break;
    }

    return szResult;
}

const wstring_view Segment::GetSDXMemoryType() const {
    const wchar_t* szResult;

    switch (bSDXControlByte & 0x7f) {
    case 0x00:
        szResult = L"main";
        break;

    case 0x02:
        szResult = L"extended";
        break;

    default:
        szResult = L"";
        break;
    }

    return szResult;
}

void Segment::CreateMemoryBlockFromBeginToEnd() {
    if (wEnd < wBegin) {
        throw std::runtime_error("End must not be before begin");
    }
    const Memory::size size = wEnd - wBegin + 1;
    CreateMemoryBlockWithSize(size);
}

void Segment::CreateMemoryBlockWithSize(Memory::size size) {
    // Free anything that is already there.
    ClearMemoryBlock();
    memoryBlock.Create(size);
}

void Segment::CreateMemoryBlockFromFile(Memory::size size, InputStream& inputStream) {
    CreateMemoryBlockWithSize(size);
    memoryBlock.ReadData(inputStream, size);
}

void Segment::ClearMemoryBlock() {
    memoryBlock.Clear();
}

byte Segment::GetData(Memory::offset offset) const {
    return memoryBlock.GetDataAt(offset);
}

Memory::word Segment::GetWord(Memory::offset offset) const {
    return GetData(offset) | (GetData(offset + 1) << 8);
}

void Segment::SetData(Memory::offset offset, byte data) {
    memoryBlock.SetDataAt(offset, data);
}
void Segment::SetData(Memory::offset offset, const ByteSequence& data, size_t dataOffset, Memory::size dataSize) {
    memoryBlock.SetDataAt(offset, data, dataOffset, dataSize);
}

MemoryType Segment::GetType(Memory::offset offset) const {
    return memoryBlock.GetTypeAt(offset);
}

bool Segment::IsType(Memory::offset offset, MemoryType bMemoryInspectorType) const {
    return GetType(offset) == bMemoryInspectorType;
}

bool Segment::IsUnknown(Memory::offset offset) const {
    if (!IsType(offset, MemoryType::UNKNOWN)) {
        return false;
    }

    // Offset 0 is never a low/high byte
    if (offset == 0) {
        return true;
    }

    // Not a low byte and not a high byte
    if (!IsType(offset - 1, MemoryType::LOBYTE) &&
        !IsType(offset - 1, MemoryType::HIBYTE)) {
        return true;
    }

    return false;
}

void Segment::SetType(Memory::offset offset, MemoryType memoryType) {
    memoryBlock.SetTypeAt(offset, memoryType);
}

void Segment::SetType(Memory::offset offset, MemoryType memoryType, Memory::size size) {
    memoryBlock.SetTypeAt(offset, memoryType, size);
}

bool Segment::ContainsAddress(Memory::address address) const {
    return (!IsEmpty() && (wBegin <= address) && (address <= wEnd));
}

void Segment::ClearComments() {
    for (auto const& comment : comments) {
        delete comment;
    }
    comments.clear();
}

gsl::not_null<Comment*> Segment::AllocateComment() {
    auto comment = new Comment();
    comments.push_back(comment);
    return comment;
}

wstring Segment::FindComment(Memory::offset offset) const {

    for (auto const& comment : comments) {
        if (comment->GetOffset() == offset) {
            return comment->GetText();
        }
    }

    return String::Empty();
}

void Segment::DeleteComments(Memory::offset offset, Memory::size size) {

    for (auto it = comments.cbegin(); it != comments.cend();)
    {
        auto const& comment = *it;
        if ((comment->GetOffset() >= offset) && (comment->GetOffset() < (offset + size))) {
            delete comment;
            it = comments.erase(it);
        }
        else {
            it++;
        }
    }
}


void Segment::AllocateSymbol(Memory::address address, wstring_view name) {
    if (IsEmpty()) {
        throw std::runtime_error("Cannot allocate symbol in empty segment");
    }
    symbols.push_back(std::make_unique< Symbol>(address, name));

    if ((address >= wBegin) && (address < wEnd)) {
        Memory::offset offset = address - wBegin;

        if (IsUnknown(offset) || IsType(offset, MemoryType::CODE)) {
            SetType(offset++, MemoryType::SYMBOL);
            SetType(offset++, MemoryType::SYMBOL);

            /*
            ** check if the fix-ups is for a jsr PRINTF.
            ** Then set the type of the following bytes to STRING until the end of string character ('\0').
            */
            if ((offset > 2) && (GetData(offset - 3) == 0x20)) { // JSR opcode
                if (name == L"PRINTF") {
                    Memory::word wBytesAsLabels = 0;
                    PrintfState nPrintState = PrintfState::TEXT;

                    while (offset < GetSize()) {
                        if (IsUnknown(offset)) {
                            SetType(offset, MemoryType::STRING);
                        }

                        const auto bByte = GetData(offset);
                        offset++;
                        if (bByte == '\0') {
                            break;
                        }

                        switch (nPrintState) {
                        case PrintfState::TEXT:
                            if (bByte == '%') {
                                nPrintState = PrintfState::PERCENT;
                            }
                            break;

                        case PrintfState::PERCENT:
                            if (bByte == '%') {
                                nPrintState = PrintfState::TEXT;
                            }
                            else if (bByte == 'c' || bByte == 's' || bByte == 'p' || bByte == 'x' || bByte == 'b' || bByte == 'd' || bByte == 'e' || bByte == 'l' || bByte == 't') {
                                nPrintState = PrintfState::TEXT;
                                wBytesAsLabels += 2;
                            }
                            else {
                                nPrintState = PrintfState::FORMAT;
                                wBytesAsLabels += 2;
                            }
                            break;

                        case PrintfState::FORMAT:
                            if (bByte == '*') {
                                wBytesAsLabels += 2;
                            }
                            else if (bByte == 'c' || bByte == 's' || bByte == 'p' || bByte == 'x' || bByte == 'b' || bByte == 'd' || bByte == 'e' || bByte == 'l' || bByte == 't') {
                                nPrintState = PrintfState::TEXT;
                            }
                            break;
                        }
                    }

                    while ((wBytesAsLabels > 0) && (offset < GetSize())) {
                        if (IsType(offset, MemoryType::UNKNOWN)) {
                            SetType(offset, MemoryType::LABEL);
                        }

                        offset++;
                        wBytesAsLabels--;
                    }
                }
            }
        }
    }
}

const Symbol* Segment::FindSymbol(Memory::address wAddr) const {
    for (auto const& symbol : symbols) {
        if (symbol->GetAddress() == wAddr) {
            return symbol.get();
        }
    }
    return nullptr;
}

void Segment::ClearFixups() {
    for (auto const& fixup : fixups) {
        delete fixup;
    }
    fixups.clear();
}

Fixup* Segment::AllocateFixup(SEGMENT_INDEX labelSegmentIndex, gsl::not_null<Segment*> labelSegment, Memory::address wAddr) {
    Fixup* fixup = nullptr;

    if (!memoryBlock.IsEmpty()) {
        fixup = new Fixup();
        fixup->SetAddress(wAddr);
        fixup->SetLabelSegmentIndex(labelSegmentIndex);

        auto it = fixups.begin();
        if (it != fixups.end()) {
            const gsl::not_null<Fixup*> firstEntry = it->get();
            if ((wAddr < firstEntry->GetAddress())) {  // Smaller than first entry?

            }
            else {
                it++;

                while (it != fixups.end() && (it->get()->GetAddress() < wAddr)) {
                    it++;
                }
            }
        }

        fixups.insert(it, fixup);

        if ((wAddr >= wBegin) && (wAddr < wEnd)) { // "<" because it must be a word at offset/offset+1
            const Memory::offset offset = wAddr - wBegin;

            if (IsUnknown(offset) || IsType(offset, MemoryType::CODE)) {
                SetType(offset, MemoryType::FIXUP);
                SetType(offset + 1, MemoryType::FIXUP);
            }

            const auto adjustAddress = GetWord(offset);
            labelSegment->fixupAddressLabels.AllocateAddressLabel(adjustAddress);
        }
    }
    return fixup;
}

const Fixup* Segment::FindFixup(Memory::address address) const {
    for (auto const& fixup : fixups) {
        if (fixup->GetAddress() == address) {
            return fixup;
        }
    }

    return nullptr;
}

gsl::not_null<AddressLabelList*> Segment::GetFixupAddressLabels() {
    return &fixupAddressLabels;
}

gsl::not_null<AddressLabelList*> Segment::GetAddressLabels() {
    return &addressLabels;
}

void Segment::DefineAddressLabel(Memory::address address) {
    auto addresslabel = fixupAddressLabels.FindMutableAddressLabel(address);

    if (addresslabel) {
        addresslabel->SetAligned(true);
        return;
    }

    addresslabel = addressLabels.FindMutableAddressLabel(address);
    if (addresslabel) {
        addresslabel->SetAligned(true);
    }
}


void Segment::AlignAddressLabels(Memory::address address) {
    fixupAddressLabels.AlignAddressLabels(address);
    addressLabels.AlignAddressLabels(address);
}


void Segment::AlignRamBlkAddresses() {
    if (IsSDXRelocBlkWithoutData()) {
        fixupAddressLabels.AlignNearestAddress();
        addressLabels.AlignNearestAddress();
    }
}

void Segment::Load14(InputStream& inputStream) {

    // Allocate memory for data and for byte type.
    CreateMemoryBlockFromBeginToEnd();

    // Set segment size.
    const auto size = GetSize();

    // Read data and type from file.
    memoryBlock.ReadData(inputStream, size);
    memoryBlock.ReadType(inputStream, size);
}

void Segment::SerializeTo(XML::Element& element) const {
    SetStdStringAttribute(Title, szTitle);

    SetWordAttributeHex(Header, wHeader);
    SetWordAttributeHex(Begin, wBegin);
    SetWordAttributeHex(End, wEnd);

    SetBoolAttribute(Binary, bBinary);
    SetStdStringAttribute(LabelPrefix, szLabelPrefix);

    SetWordAttribute(SDXFixUpSize, wSDXFixUpSize);
    SetByteAttribute(SDXBlockNumber, bSDXBlockNumber);
    SetByteAttributeHex(SDXControlByte, bSDXControlByte);
    SetStdStringAttribute(SDXSymbol, szSDXSymbol);

    SetStdStringAttribute(ProcessorType, ProcessorTypeFactory::GetInfo(processorType).key);

    auto const& contentElement = XML::AddChildElement(element, L"Content");
    memoryBlock.SerializeTo(*contentElement);

    auto const& commentsElement = XML::AddChildElement(element, L"Comments");
    for (auto const& comment : comments) {
        comment->SerializeTo(*XML::AddChildElement(*commentsElement, L"Comment"));
    }

    // Sort and serialize transient elements for debugging

    //auto const& symbolsElement = XML::AddChildElement(element, L"Symbols");
    //std::vector<Symbol*> symbolsVector;
    //for (auto const& symbol : symbols) {
    //    symbolsVector.push_back(symbol.get());
    //}
    //std::sort(symbolsVector.begin(), symbolsVector.end(), Symbol::IsAddressLess);
    //for (auto const& symbol : symbolsVector) {
    //    symbol->SerializeTo(*XML::AddChildElement(*symbolsElement, L"Symbol"));
    //}

    //std::vector<Fixup*> fixupsVector;
    //auto const& fixupsElement = XML::AddChildElement(element, L"Fixups");
    //for (auto const& fixup : fixups) {
    //    fixupsVector.push_back(fixup.get());
    //}
    //std::sort(fixupsVector.begin(), fixupsVector.end(), Fixup::IsAddressLess);
    //for (auto const& fixup : fixupsVector) {
    //    fixup->SerializeTo(*XML::AddChildElement(*fixupsElement, L"Fixup"));
    //}

    //AddressLabelList::AddressLabelVector addressLabelsVector;
    //auto const& fixupAddressLabelsElement = XML::AddChildElement(element, L"FixupAddressLabels");
    //fixupAddressLabels.Enumerate(addressLabelsVector);
    //std::sort(addressLabelsVector.begin(), addressLabelsVector.end(), AddressLabel::IsAddressLess);
    //for (auto const& addressLabel : addressLabelsVector) {
    //    addressLabel->SerializeTo(*XML::AddChildElement(*fixupAddressLabelsElement, L"AddressLabel"));
    //}
    //addressLabels.Enumerate(addressLabelsVector);
    //std::sort(addressLabelsVector.begin(), addressLabelsVector.end(), AddressLabel::IsAddressLess);
    //auto const& addressLabelsElement = XML::AddChildElement(element, L"AddressLabels");
    //for (auto const& addressLabel : addressLabelsVector) {
    //    addressLabel->SerializeTo(*XML::AddChildElement(*addressLabelsElement, L"AddressLabel"));
    //}
}

void Segment::DeserializeFrom(const XML::Element& element) {
    Clear();

    GetStdStringAttribute(Title, szTitle);

    word wHeader;
    GetWordAttribute(Header, wHeader);
    this->wHeader = (FileHeader)wHeader;
    GetWordAttribute(Begin, wBegin);
    GetWordAttribute(End, wEnd);

    GetBoolAttribute(Binary, bBinary);
    GetStdStringAttribute(LabelPrefix, szLabelPrefix);
    GetWordAttribute(SDXFixUpSize, wSDXFixUpSize);
    GetByteAttribute(SDXBlockNumber, bSDXBlockNumber);
    GetByteAttribute(SDXControlByte, bSDXControlByte);
    GetStdStringAttribute(SDXSymbol, szSDXSymbol);

    wstring processorTypeString;
    GetStdStringAttribute(ProcessorType, processorTypeString);
    processorType = ProcessorTypeFactory::GetInfo(processorTypeString).processorType;
    if (processorType == ProcessorType::UNKNOWN) {
        processorType = ProcessorType::MOS6502;
    }

    if (auto contentElement = element.FirstChildElement("Content"); contentElement) {
        memoryBlock.DeserializeFrom(*contentElement);
    }

    if (auto commentsElement = element.FirstChildElement("Comments"); commentsElement) {
        auto commentElement = commentsElement->FirstChildElement("Comment");
        while (commentElement) {
            auto const& comment = AllocateComment();
            comment->DeserializeFrom(*commentElement);
            commentElement = commentElement->NextSiblingElement();
        }
    }
}

void Segment::SetFirstLineNumber(DisassemblyResultLineNumber firstLineNumber) {
    this->firstLineNumber = firstLineNumber;
}

DisassemblyResultLineNumber  Segment::GetFirstLineNumber() const {
    return firstLineNumber;
}

