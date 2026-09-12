#pragma once

#include "Syntax.h"

#include "PartWindow.h"
#include "Window.h"


class SegmentListWindow : public PartWindow {
public:
	SegmentListWindow(Window& parentWindow, PartLayout& partLayout);

	void CreateControl(ChildID childID);

	void ClearList();
	void AddSegment(wstring_view text);

	int GetSelectedIndex() const; // Returns LB_ERR is nothing is selected
	void SetSelectedIndex(int index);
};
