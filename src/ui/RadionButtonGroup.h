#pragma once

#include "Control.h"

class RadionButtonGroup : public Control {
public:
	RadionButtonGroup(HWND hWnd) noexcept;

	RadionButtonGroup() = delete;
	RadionButtonGroup(const RadionButtonGroup& a) = delete;			// copy constructor
	RadionButtonGroup& operator=(const RadionButtonGroup&) = delete;  // copy assignment
	RadionButtonGroup(RadionButtonGroup&&) = delete;                  // move constructor
	RadionButtonGroup& operator=(RadionButtonGroup&&) = delete;       // move assignment
};
