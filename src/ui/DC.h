#pragma once

#include "Font.h"
#include "Syntax.h"
#include <Windows.h>
#include <cstddef>

// Wrapper for Windows device context.
class DC {
public:

    static HBRUSH GetStockBrush(int i);

    DC(HDC hDC);
    Font* SelectFont(Font* font);
    HBRUSH SelectBrush(HBRUSH hBrush);

    void GetTextMetrics(TEXTMETRIC& textMetric);

    COLORREF SetTextColor(COLORREF color);
    COLORREF SetBkColor(COLORREF color);
    void SetBkMode(int mode);

    void TextOut(int x, int y, const wchar_t* text, size_t length);
    void TextOut(int x, int y, wstring_view text);
    void ExtTextOut(int x, int y, const RECT& rect, wstring_view text);

    void FillRect(const RECT& rect, HBRUSH hBrush);

public: // TODO: Make private
    HDC hDC;

};