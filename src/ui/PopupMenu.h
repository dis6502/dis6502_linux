#pragma once

//#include "Point.h"
#include "Syntax.h"
#include <Windows.h>

class Menu;
class Window;


class PopupMenu {
public:

    using ItemID = unsigned int;
    using Position = unsigned int;

    virtual void CreateControl();

    void AddEntry(ItemID itemID, wstring_view newItemText);

    void AddSeparator();
    void DeleteSepartor(Position position);

    void InsertSubMenuAtPosition(Position position, PopupMenu& subMenu, wstring_view newItemText);
    void DeleteEntry(ItemID itemID);

    void SetText(ItemID itemID, wstring_view newItemText);
    void SetEnabled(ItemID itemID, bool enabled);
    void SetChecked(ItemID itemID, bool checked);

    // Track a current cursor position or at special position
    bool Track(const Window& window);
    bool Track(const Window& window, POINT pt);

    PopupMenu(const PopupMenu& a) = delete;           // no copy constructor
    PopupMenu& operator=(const PopupMenu&) = delete;  // no copy assignment
    PopupMenu(PopupMenu&&) = delete;                  // no move constructor
    PopupMenu& operator=(PopupMenu&&) = delete;       // no move assignment
    virtual ~PopupMenu(); // Must be public

protected:
    PopupMenu(wstring_view menuName = L"");

private:
    Menu* menu;
    Menu* trackedMenu;
};
