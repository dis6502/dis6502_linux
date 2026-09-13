#pragma once

#include "Syntax.h"
#include <map>
#include <memory>
#include <Windows.h>


class Menu
{

public:

    Menu(HMENU hMenu);
    ~Menu();

    using ItemID = UINT;
    using Position = UINT;
    using PositionFlags = UINT;
    using TrackFlags = UINT;


    static constexpr PositionFlags BY_COMMAND = MF_BYCOMMAND;
    static constexpr PositionFlags BY_POSITION = MF_BYPOSITION;

    static Menu* GetInstance(HMENU hMenu);

    Menu* GetSubMenu(Position position) const;
    int GetMenuItemCount() const;
    void GetItemString(Position position, PositionFlags flags, wchar_t* buffer, int maxCount) const;

    void AppendItem(PositionFlags flags, UINT_PTR itemID, wstring_view text);
    void InsertSubMenu(Position position, PositionFlags flags, Menu* subMenu, wstring_view text);
    void ModifyItem(Position position, PositionFlags flags, UINT_PTR newItemID, wstring_view text);
    void DeleteItem(Position position, PositionFlags flags);
    void DestroyMenu();

    void EnableItem(ItemID itemID, bool enable);
    void CheckItem(ItemID itemID, bool check);
    void SetItemBitmaps(Position position, PositionFlags flags, HBITMAP hBitmapUnchecked, HBITMAP hBitmapChecked);

    // For popup menus.
    bool Track(TrackFlags flags, int x, int y, HWND hWndOwner);

private:
    friend class Window;

    static constexpr HMENU NULL_HMENU = 0;

    static std::map <HMENU, const std::unique_ptr<Menu>> instances;

    HMENU hMenu;
};


