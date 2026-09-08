#include "PopupMenu.h"
#include "Text.h"
#include "UIApplication.h"
#include "Window.h"

extern std::unique_ptr<UIApplication> g_UIApplication;


PopupMenu::PopupMenu(wstring_view menuName) {
    if (menuName.empty()) {
        hMenu = CreatePopupMenu();
        hTrackedMenu = hMenu;
    }
    else {
        hMenu = LoadMenu(::g_UIApplication->GetInstanceHandle(), wstring(menuName).c_str());
        hTrackedMenu = GetSubMenu(hMenu, 0);
    }
}

PopupMenu::~PopupMenu() {
    if (hMenu != NULL_HMENU) {
        DestroyMenu(hMenu);
        hMenu = NULL_HMENU;
    }
}

void PopupMenu::CreateControl() {
    // Only required if no menu name was specified in the constructor.
}

void PopupMenu::AddEntry(UINT itemID, wstring_view newItemText) {
    AppendMenu(hTrackedMenu, MF_BYCOMMAND | MF_DISABLED | MF_STRING, itemID, wstring(newItemText).c_str());
}

void PopupMenu::AddSeparator() {
    AppendMenu(hTrackedMenu, MF_SEPARATOR, NULL_UINT_PTR, L"");
}
void  PopupMenu::DeleteSepartor(UINT position) {
    DeleteMenu(hTrackedMenu, position, MF_BYPOSITION);
}

void PopupMenu::InsertSubMenuAtPosition(UINT uPosition, PopupMenu& subMenu, LPCWSTR szNewItem) {
    InsertMenu(hTrackedMenu, uPosition, MF_POPUP | MF_BYPOSITION, (UINT_PTR)subMenu.hMenu, szNewItem);
}

void PopupMenu::DeleteEntry(UINT itemID) {
    DeleteMenu(hTrackedMenu, itemID, MF_BYCOMMAND);
}

void PopupMenu::SetText(UINT itemID, wstring_view newItemText) {
    ModifyMenu(hTrackedMenu, itemID, MF_STRING, NULL_UINT_PTR, wstring(newItemText).c_str());
}


void PopupMenu::SetEnabled(UINT itemID, bool enabled) {
    EnableMenuItem(hTrackedMenu, itemID, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void PopupMenu::SetChecked(UINT itemID, bool checked) {
    CheckMenuItem(hTrackedMenu, itemID, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

bool PopupMenu::Track(const Window& window) {
    POINT pt;
    GetCursorPos(&pt);
    return Track(window, pt);
}

bool PopupMenu::Track(const Window& window, POINT pt) {
    return (TrackPopupMenu(hTrackedMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, window.GetHWnd(), nullptr) != NULL);
}
