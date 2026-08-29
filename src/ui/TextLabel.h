#pragma once

#include "Syntax.h"
#include "UI.h"
#include "Control.h"

class TextLabel : public Control {
public:
	TextLabel(HWND hWnd);

	TextLabel() = delete;
	TextLabel(const TextLabel& a) = delete;			// copy constructor
	TextLabel& operator=(const TextLabel&) = delete;  // copy assignment
	TextLabel(TextLabel&&) = delete;                  // move constructor
	TextLabel& operator=(TextLabel&&) = delete;       // move assignment
	virtual ~TextLabel() = default;

	void SetText(wstring_view text);
	wstring GetText() const;

	void SetNumber(unsigned int value);
	unsigned int GetNumber() const;
};
