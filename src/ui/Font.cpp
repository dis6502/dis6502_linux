#include "Font.h"
#include <map>
#include <memory>
#include <utility>
#include <Windows.h>

std::map <HFONT, const std::unique_ptr<Font>> Font::instances;

Font::Font(HFONT hFont) : hFont(hFont) {
    // Debug::Log(String::Format(L"Font {0} created.", std::to_wstring((unsigned long long)hFont)));

};

Font::~Font() {
    // Debug::Log(String::Format(L"Font {0} deleted.", std::to_wstring((unsigned long long)hFont)));

};

Font* Font::GetInstance(HFONT hFont) {

    if (hFont == NULL_HFONT) {
        return nullptr;
    }

    const auto& i = instances.find(hFont);
    if (i != instances.end()) {
        return i->second.get();
    }
    instances.insert(std::make_pair(hFont, std::make_unique<Font>(hFont)));
    const auto j = instances.find(hFont);
    return j->second.get();
}


