#pragma once

#include "Syntax.h"

#include "PartWindow.h"


class XRefListWindow : public PartWindow {
public:
	XRefListWindow(Window& parentWindow, PartLayout& partLayout);

	void CreateControl(ChildID childID);
};
