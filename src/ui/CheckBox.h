#pragma once

#include "Syntax.h"
#include "UI.h"
#include "Control.h"

class CheckBox : public Control {
public:
	CheckBox(HWND hWnd);

	CheckBox() = delete;
	CheckBox(const CheckBox& a) = delete;			// copy constructor
	CheckBox& operator=(const CheckBox&) = delete;  // copy assignment
	CheckBox(CheckBox&&) = delete;                  // move constructor
	CheckBox& operator=(CheckBox&&) = delete;       // move assignment
	virtual ~CheckBox() = default;

	void SetChecked(bool selected);
	bool IsChecked() const;

};
