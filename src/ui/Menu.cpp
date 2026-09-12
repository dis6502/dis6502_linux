#include "Menu.h"
#include <map>
#include <memory>
#include <utility>
#include <Windows.h>

std::map <HMENU, const std::unique_ptr<Menu>> Menu::instances;

Menu::Menu(HMENU hMenu) : hMenu(hMenu) {
};

Menu::~Menu() {
};

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


