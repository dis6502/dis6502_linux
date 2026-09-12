#include "ComputerFont.h"
#include "Font.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "systems/ComputerSystem.h"
#include <ComputerSystemType.h>
#include <exception>
#include <map>
#include <memory>
#include <stdexcept>
#include <utility>
#include <Windows.h>

// TODO: FontFactory (/ RemoveFontResource(fontFilePath.c_str());

std::map <ComputerSystemType, const std::unique_ptr<ComputerFont>> ComputerFont::instances;


ComputerFont::ComputerFont(int fontHeight) {
    this->fontHeight = fontHeight;
    font = nullptr;
    doubleHeightFont = nullptr;
}

ComputerFont::~ComputerFont() {
    try {
        DeleteFonts();
    }
    catch (const std::exception& ex) {
        auto message = String::wstring_to_utf8(String::Format(L"Error Deleting Fonts. %s", String::utf8_to_wstring(ex.what())));
        safeExit(message.c_str());
    }
}


void ComputerFont::Load(wstring_view fontName) {

    if (CreateFonts(fontHeight, fontName)) {
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

    // Load font.
    //fontFilePath = GetResourceFilePath("AtariClassic-Regular.ttf");
    //string fontName = "Atari Classic";
    //if (FileIO::FileExists(fontFilePath)) {

    // TODO: Currently TTF is not yet supported as it would required Unicode. 
    // See https://sourceforge.net/p/dis6502/bugs/33/
    // TODO: This means we can try this now!
//    auto fontFilePath = computerSystem.GetResourceFilePathByExtension(L".fon");
//    wstring fontName = L"Atari800";
//
//    if (AddFontResource(fontFilePath.c_str()) == 0) {
//        // TODO: Raise Exception
//        auto message = String::Format(L"Font file '{0}' not found.", fontFilePath);
//        MessageBoxDialog::ShowAlert(nullptr, computerSystem.GetTypeInfo()->id, message);
//        return;
//    }
//    Load(fontName);
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
    if (!result) {
        DeleteFonts();
    }

    return result;
}

void ComputerFont::DeleteFonts() {
    if (font != nullptr) {
        const BOOL bResult = DeleteObject(font->hFont);
        font = nullptr;

        if (!bResult) {
            throw std::runtime_error("Cannot delete font");
        }
    }

    if (doubleHeightFont != nullptr) {
        const BOOL result = DeleteObject(doubleHeightFont->hFont);
        doubleHeightFont = nullptr;

        if (!result) {
            throw std::runtime_error("Cannot delete double height font");
        }
    }
}
