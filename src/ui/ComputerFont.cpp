#include "ComputerFont.h"
#include "../String.h"
#include "../MessageBoxDialog.h"

ComputerFont::ComputerFont(int fontHeight) {
	this->fontHeight = fontHeight;
	hFont = NULL_HFONT;
	hDoubleHeightFont = NULL_HFONT;
}

ComputerFont::~ComputerFont() {
	try {
		DeleteFonts();
	}
	catch (const std::exception& ex) {
		try {
			// catch anything thrown within try block that derives from std::exception
			MessageBoxDialog::Show(nullptr, L"Error Deleting Fonts", String::utf8_to_wstring(ex.what()), MB_OK);
		}
		catch (const std::exception&) {}
        safeExit();
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

	MessageBoxDialog::Show(nullptr, L"Error Loading Fonts", L"Cannot load any font.", MB_OK);

    safeExit();
}


HFONT ComputerFont::GetFont(bool doubleHeight) {
	return doubleHeight ? hDoubleHeightFont : hFont;
}


bool ComputerFont::CreateFonts(int fontHeight, wstring_view fontName) {
	constexpr int doubleHeightFontFactor = 2;
	auto fontNameString = wstring(fontName);
	hFont = CreateFont(fontHeight, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, fontNameString.c_str());
	hDoubleHeightFont = CreateFont(fontHeight * doubleHeightFontFactor, 0, 0, 0, 0, 0, 0, 0, OEM_CHARSET, 0, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, fontNameString.c_str());

	const bool result = (hFont != NULL_HFONT && hDoubleHeightFont != NULL_HFONT);
	if (!result) {
		DeleteFonts();
	}

	return result;
}

void ComputerFont::DeleteFonts() {
	if (hFont != NULL_HFONT) {
		const BOOL bResult = DeleteObject(hFont);
		hFont = NULL_HFONT;

		if (!bResult) {
			throw std::runtime_error("Cannot delete font");
		}
	}

	if (hDoubleHeightFont != NULL_HFONT) {
		const BOOL result = DeleteObject(hDoubleHeightFont);
		hDoubleHeightFont = NULL_HFONT;

		if (!result) {
			throw std::runtime_error("Cannot delete double height font");
		}
	}
}
