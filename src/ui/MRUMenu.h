#pragma once

#include "Syntax.h"
#include "UI.h"
#include "MRUEntry.h"

#include <vector>

class MRUList;

class MRUMenu
{
public:
	MRUMenu(UINT menuPosition, std::vector<UINT> itemIDList);
	~MRUMenu() = default;

	void FillMenu(HMENU hMenu, const MRUList& mruList);
	const MRUEntry* GetMRUEntry(HMENU hMenu, UINT itemID) const;

private:
	const UINT menuPosition;
	const std::vector<UINT> itemIDList;

	class MenuEntry {
	public:
		wstring menuText;
		const MRUEntry* mruEntry;
	};

	std::vector<MenuEntry*> menuEntries;
};

