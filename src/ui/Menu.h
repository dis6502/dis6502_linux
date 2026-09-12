#pragma once

#include <map>
#include <memory>
#include <Windows.h>


class Menu
{

public:

    Menu(HMENU hMenu);
    ~Menu();

    static Menu* GetInstance(HMENU hMenu);
    HMENU hMenu;

private:
    friend class Window;
    friend class MRUMenu; // TODO

    static constexpr HMENU NULL_HMENU = 0;

    static std::map <HMENU, const std::unique_ptr<Menu>> instances;



};


