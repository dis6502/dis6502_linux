#include "Menu.h"
#include "Point.h"
#include "PopupMenu.h"
#include "Syntax.h"
#include "UIApplication.h"
#include "Window.h"
#include <cstddef>
#include <memory>
#include <Windows.h>

static constexpr UINT_PTR NULL_UINT_PTR = 0;

extern std::unique_ptr<UIApplication> g_UIApplication;


PopupMenu::PopupMenu(wstring_view menuName) {
    if (menuName.empty()) {
        menu = Menu::GetInstance(CreatePopupMenu());
        trackedMenu = menu;
    }
    else {
        menu = Menu::GetInstance(LoadMenu(::g_UIApplication->GetInstanceHandle(), wstring(menuName).c_str()));
        trackedMenu = menu->GetSubMenu(0);
    }
}

PopupMenu::~PopupMenu() {
    if (menu != nullptr) {
        menu->DestroyMenu();
        menu = nullptr;
    }
}

void PopupMenu::CreateControl() {
    // Only required if no menu name was specified in the constructor.
}

void PopupMenu::AddEntry(ItemID itemID, wstring_view newItemText) {
    trackedMenu->AppendItem(MF_BYCOMMAND | MF_DISABLED | MF_STRING, itemID, newItemText);
}

void PopupMenu::AddSeparator() {
    trackedMenu->AppendItem(MF_SEPARATOR, NULL_UINT_PTR, L"");
}
void  PopupMenu::DeleteSepartor(Position position) {
    trackedMenu->DeleteItem(position, MF_BYPOSITION);
}

void PopupMenu::InsertSubMenuAtPosition(Position position, PopupMenu& subMenu, wstring_view newItemText) {
    trackedMenu->InsertSubMenu(position, MF_POPUP | MF_BYPOSITION, subMenu.menu, newItemText);
}

void PopupMenu::DeleteEntry(ItemID itemID) {
    trackedMenu->DeleteItem(itemID, MF_BYCOMMAND);
}

void PopupMenu::SetText(ItemID itemID, wstring_view newItemText) {
    trackedMenu->ModifyItem(itemID, MF_STRING, NULL_UINT_PTR, newItemText);
}


void PopupMenu::SetEnabled(ItemID itemID, bool enabled) {
    trackedMenu->EnableItem(itemID, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void PopupMenu::SetChecked(ItemID itemID, bool checked) {
    trackedMenu->CheckItem(itemID, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

bool PopupMenu::Track(const Window& window) {
    POINT pt;
    GetCursorPos((LPPOINT)&pt);
    return Track(window, pt);
}

bool PopupMenu::Track(const Window& window, POINT pt) {
    return trackedMenu->Track(TPM_LEFTALIGN, pt.x, pt.y, window.GetHWnd());
}
