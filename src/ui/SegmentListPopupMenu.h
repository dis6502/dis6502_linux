#pragma once


#include "PopupMenu.h"
#include "SegmentTypes.h"

class SegmentListPopupMenu : public PopupMenu {
public:
	SegmentListPopupMenu();

	void Update(SEGMENT_COUNT segmentCount, SEGMENT_INDEX selectedIndex);
};
