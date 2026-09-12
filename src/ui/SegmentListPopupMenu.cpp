#include "PopupMenu.h"
#include "Resource.h"
#include "SegmentTypes.h"
#include "SegmentListPopupMenu.h"


SegmentListPopupMenu::SegmentListPopupMenu() : PopupMenu(L"SEGMENT_LIST_POPUP_MENU") {
}

void SegmentListPopupMenu::Update(SEGMENT_COUNT segmentCount, SEGMENT_INDEX selectedIndex) {
	const bool selected = (selectedIndex >= 0);

	SetEnabled(IDM_SEGMENT_UP, (selectedIndex > 0));
	SetEnabled(IDM_SEGMENT_DOWN, MF_BYCOMMAND | (selectedIndex < (segmentCount - 1)));
	SetEnabled(IDM_SEGMENT_MERGE, (segmentCount > 1));
	SetEnabled(IDM_SEGMENT_DELETE, selected);
	SetEnabled(IDM_SEGMENT_SAVE_NO_HEADER, selected);
	SetEnabled(IDM_SEGMENT_SAVE_HEADER, selected);
	SetEnabled(IDM_SEGMENT_SAVE_ALL, (segmentCount > 0));
	SetEnabled(IDM_SEGMENT_PROPS, selected);
}
