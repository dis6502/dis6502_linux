#include "ComputerFont.h"
#include "Font.h"
#include "Syntax.h"
#include "systems/ComputerSystem.h"
#include <algorithm>
#include <ComputerSystemType.h>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <Windows.h>

std::map <ComputerSystemType, const std::unique_ptr<ComputerFont>> ComputerFont::instances;

namespace {

    int CALLBACK CollectFaceName(const LOGFONTW* logFont, const TEXTMETRICW*, DWORD, LPARAM lParam) {
        static_cast<std::vector<wstring>*>((void*)lParam)->push_back(logFont->lfFaceName);
        return 1;
    }

    // Enumerates the face names of all fonts currently known to GDI, so a newly
    // added private font resource can be identified by diffing before and after.
    std::vector<wstring> GetInstalledFaceNames() {
        std::vector<wstring> faceNames;
        LOGFONTW logFont{};
        logFont.lfCharSet = DEFAULT_CHARSET;
        const auto hDC = GetDC(NULL);
        EnumFontFamiliesExW(hDC, &logFont, CollectFaceName, (LPARAM)&faceNames, 0);
        ReleaseDC(NULL, hDC);
        return faceNames;
    }

}


ComputerFont::ComputerFont(int fontHeight) {
    this->fontHeight = fontHeight;
    font = nullptr;
    doubleHeightFont = nullptr;
}

ComputerFont::~ComputerFont() {}


void ComputerFont::Load(wstring_view fontFilePath) {

    const auto faceName = RegisterFontResource(fontFilePath);
    if (!faceName.empty() && CreateFonts(fontHeight, faceName)) {
        return;
    }

    //// fallback to use fonts under Wine
    //if (CreateFonts(fontHeight, "Atari Classic Chunky")) {
    //	return;
    //}
    //if (CreateFonts(fontHeight, "Atari Classic Smooth")) {
    //	return;
    //}
    //if (CreateFonts(fontHeight, "Atari Classic Extrasmooth")) {
    //	return;
    //}

    if (CreateFonts(fontHeight, L"Courier New")) {
        return;
    }

    throw std::runtime_error("Cannot load any font.");
}

wstring ComputerFont::RegisterFontResource(wstring_view fontFilePath) {
    const auto faceNamesBefore = GetInstalledFaceNames();

    if (AddFontResourceExW(wstring(fontFilePath).c_str(), FR_PRIVATE, 0) == 0) {
        return wstring(); // Font file not found or invalid.
    }

    for (const auto& faceName : GetInstalledFaceNames()) {
        if (std::find(faceNamesBefore.begin(), faceNamesBefore.end(), faceName) == faceNamesBefore.end()) {
            return faceName;
        }
    }

    return wstring(); // Registered, but the resulting face name could not be determined.
}

const ComputerFont& ComputerFont::Get(const ComputerSystem& computerSystem) {

    const auto& i = instances.find(computerSystem.GetType());
    if (i != instances.end()) {
        return *(i->second);
    }
    instances.insert(std::make_pair(computerSystem.GetType(), std::make_unique<ComputerFont>(8)));
    auto j = instances.find(computerSystem.GetType());
    auto fontFilePath = computerSystem.GetResourceFilePathByExtension(L".fon");
    j->second->Load(fontFilePath);
    return *(j->second);
}


Font* ComputerFont::GetFont(bool doubleHeight) const {
    return doubleHeight ? doubleHeightFont : font;
}


bool ComputerFont::CreateFonts(int fontHeight, wstring_view fontName) {
    constexpr int doubleHeightFontFactor = 2;
    auto fontNameString = wstring(fontName);
    font = Font::GetInstance(CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, fontNameString.c_str()));
    doubleHeightFont = Font::GetInstance(CreateFont(fontHeight * doubleHeightFontFactor, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, fontNameString.c_str()));

    const bool result = (font != nullptr && doubleHeightFont != nullptr);
    return result;
}

