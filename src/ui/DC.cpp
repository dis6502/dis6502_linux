#include "DC.h"


HBRUSH DC::GetStockBrush(int i) {
    return ((HBRUSH)GetStockObject(i));
}

DC::DC(HDC hDC) : hDC(hDC) {
    if (hDC == NULL) {
        throw std::runtime_error("Cannot create device context");
    }
}

HFONT DC::SelectFont(HFONT hFont) {
    return (HFONT)::SelectObject(hDC, hFont);
}
HBRUSH DC::SelectBrush(HBRUSH hBrush) {
    return (HBRUSH)::SelectObject(hDC, hBrush);
}

void DC::GetTextMetrics(TEXTMETRIC& textMetric) {
    ::GetTextMetrics(hDC, &textMetric);
}

COLORREF DC::SetTextColor(COLORREF color) {
    return ::SetTextColor(hDC, color);
}

COLORREF DC::SetBkColor(COLORREF color) {
    return ::SetBkColor(hDC, color);
}

void DC::SetBkMode(int mode) {
    ::SetBkMode(hDC, mode);
}

void DC::TextOut(int x, int y, const wchar_t* text, size_t length) {
    ::TextOut(hDC, x, y, text, length);
}


void DC::TextOut(int x, int y, wstring_view text) {
    TextOut(x, y, text.data(), text.length());
}

void DC::ExtTextOut(int x, int y, const RECT& rect, wstring_view text) {
    ::ExtTextOut(hDC, x, y, ETO_CLIPPED | ETO_OPAQUE, &rect, text.data(), text.length(), nullptr);
}

void DC::FillRect(const RECT& rect, HBRUSH hBrush) {
    ::FillRect(hDC, &rect, hBrush);
}

