#include "Pass1.h"

#include "Workspace.h"
#include "SegmentList.h"
#include "MemoryBlockIterator.h"


void Pass1::Execute(Workspace& workspace) {
    Memory::address wStartAddr = 0;

    const auto segmentList = workspace.GetSegmentList();
    for (SEGMENT_INDEX segmentIndex = 0; segmentIndex < segmentList->GetCount(); segmentIndex++) {
        const auto segment = segmentList->GetConstSegment(segmentIndex);
        if (segment->IsHeader(FileHeader::SDX_SYM_DEFINED)
            || segment->IsSDXRelocBlkWithoutData()) { // TODO: With or Without? Original code : if ((lpSegment->IsHeader(SDX_RELOC_BLK)) && (lpSegment->bSDXControlByte & 0x80)) 

            auto labelSegment = segmentList->FindBySDXBlockNumber(segment->bSDXBlockNumber);

            if (labelSegment != nullptr) {
                wStartAddr = segment->wBegin;
                labelSegment->GetFixupAddressLabels()->AllocateAddressLabel(wStartAddr);
            }
        }
        else if ((segment->IsHeader(FileHeader::SDX_SYM_REQUIRED) || (segment->IsHeader(FileHeader::SDX_FIX_UP_BLK)))) {
        Segment* blockSegment = nullptr;
            wStartAddr = 0;
            byte bSDXBlockNumber = 1;

            auto i = MemoryBlockIterator(segment->memoryBlock);
        while (i.HasNext() && i.GetData() != static_cast<byte>(FixupType::END)) {
            auto data = i.NextData();
            const auto fixup = static_cast<FixupType>(data);
            switch (fixup) {
            case FixupType::ADD_250_BYTES:
                wStartAddr += 250;
                break;

            case FixupType::SET_BLOCK_NUM:
                bSDXBlockNumber = i.NextData();
                blockSegment = segmentList->FindBySDXBlockNumber(bSDXBlockNumber);
                if (blockSegment != nullptr) {
                    wStartAddr = blockSegment->wBegin;
                }
                break;

            case FixupType::SET_BLOCK_ADDR:
                wStartAddr = i.NextAddress();
                {
                    const auto iSegmentIndex = segmentList->FindByAddr(wStartAddr);
                    if (iSegmentIndex != NO_SEGMENT_INDEX) {
                        blockSegment = segmentList->GetSegment(iSegmentIndex);
                    }
                    else {
                        blockSegment = nullptr;
                    }
                }
                data = 0;
                // fall thru !

            default:
                wStartAddr += data;
                if (blockSegment != nullptr) {
                    if (segment->IsHeader(FileHeader::SDX_SYM_REQUIRED)) {
                        blockSegment->AllocateSymbol(wStartAddr, segment->szSDXSymbol);
                    }
                    else if (segment->IsHeader(FileHeader::SDX_FIX_UP_BLK)) {
                        auto labelSegment = segmentList->FindBySDXBlockNumber(segment->bSDXBlockNumber);
                        if (labelSegment != nullptr) {
                            blockSegment->AllocateFixup(segmentList->GetSegmentIndex(labelSegment), labelSegment, wStartAddr);
                        }
                    }
                }
                break;
            }
        }
        }
}
}
