#pragma once

#include "ComputerSystemType.h"
#include "Font.h"
#include "Syntax.h"
#include <map>
#include <memory>

class ComputerSystem;

class ComputerFont {
public:
    ComputerFont() = delete;								// no default constructor
    ComputerFont(const ComputerFont& a) = delete;			// no copy constructor
    ComputerFont& operator=(const ComputerFont&) = delete;  // no copy assignment
    ComputerFont(ComputerFont&&) = delete;                  // no move constructor
    ComputerFont& operator=(ComputerFont&&) = delete;       // no move assignment
    ComputerFont(int fontHeight);
    ~ComputerFont();

    static const ComputerFont& Get(const ComputerSystem& computerSystem);

    Font* GetFont(bool doubleHeight) const;

private:
    static std::map <ComputerSystemType, const std::unique_ptr<ComputerFont>> instances;

    int fontHeight;
    Font* font;
    Font* doubleHeightFont;


    void Load(wstring_view fontName);
    bool CreateFonts(int fontHeight, wstring_view fontName);
};
