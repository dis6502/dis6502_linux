#include "MRUEntry.h"
#include "MRUList.h"
#include "MRUMenu.h"
#include "StringUtility.h"
#include <sstream>

MRUMenu::MRUMenu(UINT menuPosition, std::vector<UINT> itemIDList) :
    menuPosition(menuPosition), itemIDList(itemIDList) {}

void MRUMenu::FillMenu(HMENU hMenu, const MRUList& mruList) {

    const auto entries = mruList.GetEntries();
    // Enable menu item.
    EnableMenuItem(hMenu, menuPosition, MF_BYPOSITION | (!entries.empty() ? MF_ENABLED : MF_GRAYED));

    // Fill sub menu.
    auto hSubMenu = GetSubMenu(hMenu, menuPosition);
    if (hSubMenu == NULL_HMENU) {
        throw std::exception("No submenu for MRU at specified position");
    }

    auto index = GetMenuItemCount(hSubMenu);
    while (index > 0) {
        DeleteMenu(hSubMenu, 0, MF_BYPOSITION);
        index--;
    }

    for (auto entry : entries) {
        index++;
        MenuEntry* menuEntry = new MenuEntry();
        std::wostringstream ss;
        ss << L"&" << index << L" " << entry->GetFilePath();
        menuEntry->menuText = ss.str();
        menuEntry->mruEntry = entry;
        menuEntries.push_back(menuEntry);
        AppendMenu(hSubMenu, MF_ENABLED | MF_STRING, itemIDList.at(index), menuEntry->menuText.c_str());
    }
}


const MRUEntry* MRUMenu::GetMRUEntry(HMENU hMenu, UINT itemID) const {
    constexpr int MAX_LENGTH = 3 + _MAX_PATH;
    std::unique_ptr<wchar_t[]> szBuffer = std::make_unique<wchar_t[]>(MAX_LENGTH);
    auto hSubMenu = GetSubMenu(hMenu, menuPosition);
    GetMenuString(hSubMenu, itemID, szBuffer.get(), MAX_LENGTH - 1, MF_BYCOMMAND);

    for (auto menuEntry : menuEntries) {
        if (String::EqualsIgnoreCase(menuEntry->menuText, szBuffer.get())) {
            return menuEntry->mruEntry;
        }
    }

    return nullptr;
}