#pragma once

#include "Syntax.h"
#include "UI.h"
#include <vector>

class MainWindowMenu {
public:
	MainWindowMenu();
	MainWindowMenu(const MainWindowMenu& a) = delete;			// copy constructor
	MainWindowMenu& operator=(const MainWindowMenu&) = delete;  // copy assignment
	MainWindowMenu(MainWindowMenu&&) = delete;                  // move constructor
	MainWindowMenu& operator=(MainWindowMenu&&) = delete;       // move assignment
	virtual ~MainWindowMenu();

	void CreateControl(HWND hMainWnd);
	HMENU GetSubMenuHandle(UINT uMenuPos) const;

	void SetEnabled(UINT uIDEnableItem, bool enabled);
	void SetChecked(UINT uIDCheckItem, bool checked);

private:
	HMENU hMenu;
	std::vector<HBITMAP> bitmaps;

	HBITMAP MakeBitMapTransparent(HBITMAP hbmSrc);
	void AddIconToMenu(UINT uPosition, int nIconId);
};
