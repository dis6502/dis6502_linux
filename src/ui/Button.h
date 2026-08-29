#pragma once

#include "Syntax.h"
#include "UI.h"
#include "Control.h"

class Button : public Control {
public:
	Button(HWND hWnd);

	Button() = delete;
	Button(const Button& a) = delete;			// copy constructor
	Button& operator=(const Button&) = delete;  // copy assignment
	Button(Button&&) = delete;                  // move constructor
	Button& operator=(Button&&) = delete;       // move assignment
};
