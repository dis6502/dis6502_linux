#include "Font.h"
#include <map>
#include <memory>
#include <utility>
#include <Windows.h>

std::map <HFONT, const std::unique_ptr<Font>> Font::instances;

Font::Font() {};

Font::~Font() {
    if (hFont != NULL_HFONT) {
        DeleteObject(hFont);
    }

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
    instances.insert(std::make_pair(hFont, std::make_unique<Font>()));
    const auto j = instances.find(hFont);
    auto result = j->second.get();
    result->hFont = hFont;
    return result;
}


