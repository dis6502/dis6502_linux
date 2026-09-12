#include "Control.h"
#include "StringUtility.h"
#include "TextLabel.h"
#include <string>
#include "Syntax.h"
#include <wchar.h>

TextLabel::TextLabel(WindowHandle hWnd) : Control(hWnd) {}


void TextLabel::SetText(wstring_view text) {
    static wchar_t buffer[1024];
    wcsncpy_s(buffer, sizeof(buffer), text.data(), text.size());
    SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)buffer);
}

wstring TextLabel::GetText() const {
    static wchar_t buffer[1024];
    strclr(buffer);
    SendMessage(hWnd, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
    return wstring(buffer);
}

void TextLabel::SetNumber(unsigned int value) {
    static wchar_t szNumber[11]; //  4294967295
    String::Printf(szNumber, L"%u", value);
    SetText(szNumber);
}

unsigned int TextLabel::GetNumber() const {
    unsigned int value = 0;
    auto text = GetText();
    swscanf(text.c_str(), L"%u", &value);
    return value;
}
