#pragma once

#include "../Syntax.h"
#include "../UI.h"


class ComputerFont {
public:
	ComputerFont() = delete;								// no default constructor
	ComputerFont(const ComputerFont& a) = delete;			// no copy constructor
	ComputerFont& operator=(const ComputerFont&) = delete;  // no copy assignment
	ComputerFont(ComputerFont&&) = delete;                  // no move constructor
	ComputerFont& operator=(ComputerFont&&) = delete;       // no move assignment

	ComputerFont(int fontHeight);
	~ComputerFont();

	void Load(wstring_view fontName);

	HFONT GetFont(bool doubleHeight);

private:
	int fontHeight;
	HFONT hFont;
	HFONT hDoubleHeightFont;

	bool CreateFonts(int fontHeight, wstring_view fontName);
	void DeleteFonts();
};
