#pragma once

#include "MemoryType.h"
#include "PopupMenu.h"
#include <utility>
#include <vector>

class Workspace;
class SegmentList;
class MemoryInspectorSelection;


class MemoryInspectorPopupMenu : public PopupMenu {

    class TypeSubMenu {
    public:
        TypeSubMenu();

        void Update(MemoryInspectorPopupMenu& menu, const MemoryInspectorSelection& memoryInspectorSelection);
    private:
        std::vector<std::pair<UINT_PTR, MemoryType>> entryMappingList;

    };

public:
    MemoryInspectorPopupMenu();

    void Update(const SegmentList& segmentList, const MemoryInspectorSelection& memoryInspectorSelection, bool findEnabled, bool findNextEnabled);

private:
    TypeSubMenu typeSubMenu;
};


