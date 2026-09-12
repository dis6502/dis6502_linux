#include "FileIO.h"
#include "Menu.h"
#include "MRUEntry.h"
#include "MRUList.h"
#include "MRUMenu.h"
#include "StringUtility.h"
#include "UI.h"
#include <exception>
#include <memory>
#include <sstream>
#include <vector>
#include <Windows.h>

MRUMenu::MRUMenu(UINT menuPosition, std::vector<UINT> itemIDList) :
    menuPosition(menuPosition), itemIDList(itemIDList) {}

MRUMenu::~MRUMenu() {
    ClearMenuEntries();
}

void MRUMenu::ClearMenuEntries() {
    for (auto menuEntry : menuEntries) {
        delete menuEntry;
    }
    menuEntries.clear();
}

void MRUMenu::FillMenu(Menu* menu, const MRUList& mruList) {

    ClearMenuEntries();

    const auto entries = mruList.GetEntries();
    // Enable menu item.
    EnableMenuItem(menu->hMenu, menuPosition, MF_BYPOSITION | (!entries.empty() ? MF_ENABLED : MF_GRAYED));

    // Fill sub menu.
    auto hSubMenu = GetSubMenu(menu->hMenu, menuPosition);
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


const MRUEntry* MRUMenu::GetMRUEntry(Menu* menu, UINT itemID) const {
    constexpr int MAX_LENGTH = 3 + FileIO::FILE_PATH_SIZE;
    std::unique_ptr<wchar_t[]> szBuffer = std::make_unique<wchar_t[]>(MAX_LENGTH);
    auto hSubMenu = GetSubMenu(menu->hMenu, menuPosition);
    GetMenuString(hSubMenu, itemID, szBuffer.get(), MAX_LENGTH - 1, MF_BYCOMMAND);

    for (auto menuEntry : menuEntries) {
        if (String::EqualsIgnoreCase(menuEntry->menuText, szBuffer.get())) {
            return menuEntry->mruEntry;
        }
    }

    return nullptr;
}