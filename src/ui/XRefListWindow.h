#pragma once


#include "PartWindow.h"
#include "Window.h"


class XRefListWindow : public PartWindow {
public:
	XRefListWindow(Window& parentWindow, PartLayout& partLayout);

	void CreateControl(ChildID childID);
};
