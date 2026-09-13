#include "FileIO.h"
#include "Menu.h"
#include "MRUEntry.h"
#include "MRUList.h"
#include "MRUMenu.h"
#include "StringUtility.h"
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
    menu->EnableItem(menuPosition, MF_BYPOSITION | (!entries.empty() ? MF_ENABLED : MF_GRAYED));

    // Fill sub menu.
    auto subMenu = menu->GetSubMenu(menuPosition);
    if (subMenu == nullptr) {
        throw std::exception("No submenu for MRU at specified position");
    }

    auto index = subMenu->GetMenuItemCount();
    while (index > 0) {
        subMenu->DeleteItem(0, MF_BYPOSITION);
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
        subMenu->AppendItem(MF_ENABLED | MF_STRING, itemIDList.at(index), menuEntry->menuText);
    }
}


const MRUEntry* MRUMenu::GetMRUEntry(Menu* menu, UINT itemID) const {
    constexpr int MAX_LENGTH = 3 + FileIO::FILE_PATH_SIZE;
    std::unique_ptr<wchar_t[]> szBuffer = std::make_unique<wchar_t[]>(MAX_LENGTH);
    auto subMenu = menu->GetSubMenu(menuPosition);
    subMenu->GetItemString(itemID, MF_BYCOMMAND, szBuffer.get(), MAX_LENGTH - 1);

    for (auto menuEntry : menuEntries) {
        if (String::EqualsIgnoreCase(menuEntry->menuText, szBuffer.get())) {
            return menuEntry->mruEntry;
        }
    }

    return nullptr;
}