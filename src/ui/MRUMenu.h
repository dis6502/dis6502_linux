#pragma once

#include "Syntax.h"
#include "MRUEntry.h"

#include <vector>
#include <Windows.h>

class Menu;
class MRUList;

class MRUMenu
{
public:
	MRUMenu(UINT menuPosition, std::vector<UINT> itemIDList);
	~MRUMenu();

	void FillMenu(Menu* menu, const MRUList& mruList);
	const MRUEntry* GetMRUEntry(Menu* menu, UINT itemID) const;

private:
	const UINT menuPosition;
	const std::vector<UINT> itemIDList;

	class MenuEntry {
	public:
		wstring menuText;
		const MRUEntry* mruEntry;
	};

	std::vector<MenuEntry*> menuEntries;

	void ClearMenuEntries();
};

