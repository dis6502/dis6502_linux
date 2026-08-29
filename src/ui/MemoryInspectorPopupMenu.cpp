#include "MemoryInspectorPopupMenu.h"
#include "MemoryInspectorSelection.h"
#include "Resource.h"

#include "Segment.h"
#include "SegmentList.h"
#include "Workspace.h"
#include <gsl/util>
#include <vector>

extern std::unique_ptr<Workspace> g_Workspace;

MemoryInspectorPopupMenu::TypeSubMenu::TypeSubMenu() {
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_CODE, MemoryType::CODE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_LOBYTE, MemoryType::LOBYTE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_HIBYTE, MemoryType::HIBYTE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_BYTE, MemoryType::BYTE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_WORD, MemoryType::WORD));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_LABEL, MemoryType::LABEL));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_SYMBOL, MemoryType::SYMBOL));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_FIXUP, MemoryType::FIXUP));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_STRING, MemoryType::STRING));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_SBYTE, MemoryType::SBYTE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_DLIST, MemoryType::DLIST));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_STORE, MemoryType::STORE));
    entryMappingList.push_back(std::pair<UINT_PTR, MemoryType>(IDM_DUMP_SET_TYPE_UNKNOWN, MemoryType::UNKNOWN));
}

void MemoryInspectorPopupMenu::TypeSubMenu::Update(MemoryInspectorPopupMenu& menu, const MemoryInspectorSelection& memoryInspectorSelection) {
    const auto size = entryMappingList.size();
    std::vector<bool> enabledArray;
    std::vector<bool> presentArray;
    enabledArray.resize(size, false);
    presentArray.resize(size, false);

    for (int i = 0; i < size; i++) {
        const auto entryMapping = entryMappingList.at(i);
        // Entries are enabled if there is a selection.
        auto enabled = memoryInspectorSelection.HasSelection();
        if (enabled) {
            const MemoryType memoryType = entryMapping.second;

            // LOBYTE and HIBYTE cannot be set for the first byte of a segment
            if (memoryType == MemoryType::LOBYTE || memoryType == MemoryType::HIBYTE) {
                enabled = (memoryInspectorSelection.GetBegin() > 0);
            }
        }

        enabledArray[i] = enabled;
        presentArray[i] = false;
    }

    if (memoryInspectorSelection.HasSelection()) {
        for (auto offset = memoryInspectorSelection.GetBegin(); offset <= memoryInspectorSelection.GetEnd(); offset++) {
            auto memoryType = memoryInspectorSelection.segment->GetType(offset);


            if ((memoryType > MemoryType::FIXUP) || (memoryType == MemoryType::UNKNOWN)) {
                if (offset == 0) {
                    memoryType = MemoryType::UNKNOWN;
                }
                else if (memoryInspectorSelection.segment->IsType(offset - 1, MemoryType::LOBYTE)) {
                    memoryType = MemoryType::LOBYTE;
                }
                else if (memoryInspectorSelection.segment->IsType(offset - 1, MemoryType::HIBYTE)) {
                    memoryType = MemoryType::HIBYTE;
                }
                else {
                    memoryType = MemoryType::UNKNOWN;
                }
            }

            for (int i = 0; i < size; i++) {
                if (entryMappingList.at(i).second == memoryType) {
                    presentArray[i] = true;
                }
            }
        }
    }

    for (int i = 0; i < size; i++) {
        const auto entryMapping = entryMappingList.at(i);
        menu.SetEnabled(entryMapping.first, enabledArray[i]);
        menu.SetChecked(entryMapping.first, presentArray[i]);
    }
}

MemoryInspectorPopupMenu::MemoryInspectorPopupMenu() :PopupMenu(L"MEMORY_INSPECTOR_POPUP_MENU") {

}

void MemoryInspectorPopupMenu::Update(const SegmentList& segmentList, const MemoryInspectorSelection& memoryInspectorSelection, bool findEnabled, bool findNextEnabled) {
    const auto bHasSegment = memoryInspectorSelection.HasSegment();
    const auto bHasSelection = memoryInspectorSelection.HasSelection();
    const auto wBegin = memoryInspectorSelection.GetBegin();

    SetEnabled(IDM_DUMP_START_CODE_TRACE, (bHasSelection && (memoryInspectorSelection.segment->IsType(wBegin, MemoryType::UNKNOWN))));

    SetEnabled(IDM_DUMP_SET_UNKNOWN_BLOCK_TO_BYTE, bHasSelection);

    SetEnabled(IDM_DUMP_EDIT_COMMENT, bHasSelection);
    SetEnabled(IDM_DUMP_EDIT, bHasSelection);
    SetEnabled(IDM_DUMP_ASSEMBLE, bHasSelection);

    SetEnabled(IDM_DUMP_CUT_SELECTION, bHasSelection);
    SetEnabled(IDM_DUMP_COPY_SELECTION, bHasSelection);
    SetEnabled(IDM_DUMP_PASTE_AT_SELECTION, bHasSelection);
    SetEnabled(IDM_DUMP_PASTE_AFTER_SELECTION, bHasSelection);
    SetEnabled(IDM_DUMP_DELETE_SELECTION, bHasSelection);
    SetEnabled(IDM_DUMP_SPLIT_AT_SELECTION, (bHasSelection && (segmentList.GetCount() < SegmentList::MAX_SEGMENTS) && (memoryInspectorSelection.segment->CanSplitAt(wBegin))));

    SetEnabled(IDM_DUMP_FIND, findEnabled);
    SetEnabled(IDM_DUMP_FIND_NEXT, findNextEnabled);

    SetEnabled(IDM_DUMP_SELECT_NEXT_UNKNOWN_BLOCK, bHasSegment);
    SetEnabled(IDM_DUMP_SELECT_SPRITES, bHasSegment);
    SetEnabled(IDM_DUMP_SELECT_ALL, bHasSegment);

    SetEnabled(IDM_DUMP_SAVE_NO_HEADER, bHasSelection);
    SetEnabled(IDM_DUMP_SAVE_HEADER, bHasSelection);

    typeSubMenu.Update(*this, memoryInspectorSelection);
}
