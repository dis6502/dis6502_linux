#include "Menu.h"
#include "Syntax.h"
#include <map>
#include <memory>
#include <utility>
#include <Windows.h>

std::map <HMENU, const std::unique_ptr<Menu>> Menu::instances;

Menu::Menu(HMENU hMenu) : hMenu(hMenu) {};

Menu::~Menu() {};

Menu* Menu::GetInstance(HMENU hMenu) {

    if (hMenu == NULL_HMENU) {
        return nullptr;
    }

    const auto& i = instances.find(hMenu);
    if (i != instances.end()) {
        return i->second.get();
    }
    instances.insert(std::make_pair(hMenu, std::make_unique<Menu>(hMenu)));
    const auto j = instances.find(hMenu);
    return j->second.get();
}

Menu* Menu::GetSubMenu(Position position) const {
    return Menu::GetInstance(::GetSubMenu(hMenu, position));
}

int Menu::GetMenuItemCount() const {
    return ::GetMenuItemCount(hMenu);
}

void Menu::GetItemString(Position position, PositionFlags flags, wchar_t* buffer, int maxCount) const {
    GetMenuString(hMenu, position, buffer, maxCount, flags);
}

void Menu::AppendItem(PositionFlags flags, UINT_PTR itemID, wstring_view text) {
    AppendMenu(hMenu, flags, itemID, wstring(text).c_str());
}

void Menu::InsertSubMenu(Position position, PositionFlags flags, Menu* subMenu, wstring_view text) {
    InsertMenu(hMenu, position, flags, (UINT_PTR)subMenu->hMenu, wstring(text).c_str());
}

void Menu::ModifyItem(Position position, PositionFlags flags, UINT_PTR newItemID, wstring_view text) {
    ModifyMenu(hMenu, position, flags, newItemID, wstring(text).c_str());
}

void Menu::DeleteItem(Position position, PositionFlags flags) {
    DeleteMenu(hMenu, position, flags);
}

void Menu::DestroyMenu() {
    if (hMenu != NULL_HMENU) {
        ::DestroyMenu(hMenu);
    }
}

void Menu::EnableItem(ItemID itemID, bool enable) {
    ::EnableMenuItem(hMenu, itemID, (UINT)enable);
}

void Menu::CheckItem(ItemID itemID, bool check) {
    ::CheckMenuItem(hMenu, itemID, (UINT)check);
}

void Menu::SetItemBitmaps(Position position, PositionFlags flags, HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked) {
    ::SetMenuItemBitmaps(hMenu, position, flags, hBitmapUnchecked, hBitmapChecked);
}

bool Menu::Track(TrackFlags flags, int x, int y, HWND hWndOwner) {
    return (::TrackPopupMenu(hMenu, flags, x, y, 0, hWndOwner, nullptr) != NULL);
}


