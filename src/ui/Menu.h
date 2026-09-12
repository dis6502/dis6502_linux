#pragma once

#include <map>
#include <memory>
#include <Windows.h>


class Menu
{

public:
    static constexpr HMENU NULL_HMENU = 0;

    Menu(HMENU hMenu);
    ~Menu();

    static Menu* GetInstance(HMENU hMenu);


    HMENU hMenu;

private:
    static std::map <HMENU, const std::unique_ptr<Menu>> instances;

};


