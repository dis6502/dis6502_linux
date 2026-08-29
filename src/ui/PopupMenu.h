#pragma once

#include "Syntax.h"
#include "UI.h"

class Window;


class PopupMenu {
public:
	virtual void CreateControl();

	void AddEntry(UINT itemID, wstring_view newItemText);
	
	void AddSeparator();
    void DeleteSepartor(UINT position);

	void InsertSubMenuAtPosition(UINT position, PopupMenu& subMenu, LPCWSTR szNewItem);
    void DeleteEntry(UINT itemID);

    void SetText(UINT itemID, wstring_view newItemText);
	void SetEnabled(UINT itemID, bool enabled);
	void SetChecked(UINT itemID, bool checked);

	// Track a current cursor position or at special position
	bool Track(const Window& window);
	bool Track(const Window& window, POINT pt);

    PopupMenu(const PopupMenu& a) = delete;           // no copy constructor
    PopupMenu& operator=(const PopupMenu&) = delete;  // no copy assignment
    PopupMenu(PopupMenu&&) = delete;                  // no move constructor
    PopupMenu& operator=(PopupMenu&&) = delete;       // no move assignment
    virtual ~PopupMenu(); // Must be public

protected:
	PopupMenu(wstring_view menuName = L"");

private:
	HMENU hMenu;
	HMENU hTrackedMenu;
};
