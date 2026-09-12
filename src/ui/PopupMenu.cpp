#include "PopupMenu.h"
#include "Menu.h"
#include "Syntax.h"
#include "UI.h"
#include "UIApplication.h"
#include "Window.h"
#include <Windows.h>
#include <cstddef>
#include <memory>

extern std::unique_ptr<UIApplication> g_UIApplication;


PopupMenu::PopupMenu(wstring_view menuName) {
    if (menuName.empty()) {
        menu = Menu::GetInstance(CreatePopupMenu());
        trackedMenu = menu;
    }
    else {
        menu = Menu::GetInstance(LoadMenu(::g_UIApplication->GetInstanceHandle(), wstring(menuName).c_str()));
        trackedMenu = Menu::GetInstance(GetSubMenu(menu->hMenu, 0));
    }
}

PopupMenu::~PopupMenu() {
    if (menu != nullptr) {
        DestroyMenu(menu->hMenu);
        menu = nullptr;
    }
}

void PopupMenu::CreateControl() {
    // Only required if no menu name was specified in the constructor.
}

void PopupMenu::AddEntry(ItemID itemID, wstring_view newItemText) {
    AppendMenu(trackedMenu->hMenu, MF_BYCOMMAND | MF_DISABLED | MF_STRING, itemID, wstring(newItemText).c_str());
}

void PopupMenu::AddSeparator() {
    AppendMenu(trackedMenu->hMenu, MF_SEPARATOR, NULL_UINT_PTR, L"");
}
void  PopupMenu::DeleteSepartor(Position position) {
    DeleteMenu(trackedMenu->hMenu, position, MF_BYPOSITION);
}

void PopupMenu::InsertSubMenuAtPosition(Position position, PopupMenu& subMenu, wstring_view newItemText) {
    InsertMenu(trackedMenu->hMenu, position, MF_POPUP | MF_BYPOSITION, (UINT_PTR)subMenu.menu->hMenu, wstring(newItemText).c_str());
}

void PopupMenu::DeleteEntry(ItemID itemID) {
    DeleteMenu(trackedMenu->hMenu, itemID, MF_BYCOMMAND);
}

void PopupMenu::SetText(ItemID itemID, wstring_view newItemText) {
    ModifyMenu(trackedMenu->hMenu, itemID, MF_STRING, NULL_UINT_PTR, wstring(newItemText).c_str());
}


void PopupMenu::SetEnabled(ItemID itemID, bool enabled) {
    EnableMenuItem(trackedMenu->hMenu, itemID, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void PopupMenu::SetChecked(ItemID itemID, bool checked) {
    CheckMenuItem(trackedMenu->hMenu, itemID, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

bool PopupMenu::Track(const Window& window) {
    POINT pt;
    GetCursorPos(&pt);
    return Track(window, pt);
}

bool PopupMenu::Track(const Window& window, POINT pt) {
    return (TrackPopupMenu(trackedMenu->hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, window.GetHWnd(), nullptr) != NULL);
}
