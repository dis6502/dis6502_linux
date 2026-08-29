#pragma once

#include "Syntax.h"
#include "UI.h"

class Control {
public:

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	Control(HWND hWnd);
	virtual ~Control();

	void SetEnabled(bool enabled);
	void SetFont(const HFONT& hFont);

    bool HasFocus() const;
	void SetFocus();

    void SetScrollRange(int bar, ScrollPosition minPosition,ScrollPosition maxPosition, BOOL update);
    void SetScrollPosition(int bar, ScrollPosition scrollPosition, BOOL update);

	HWND hWnd;

protected:
	virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	static Control* GetInstance(HWND hWND);

};
