#include "Application.h"

#include "EditControl.h"
#include "StringUtility.h"

extern Application* g_Application;

EditControl::EditControl(HWND hWnd) : Control(hWnd) {}

void EditControl::Clear() {
    SetText(L"");
}

void EditControl::SetTextSize(int size) {
    SendMessage(hWnd, EM_LIMITTEXT, size, 0L);
}

void EditControl::SetText(wstring_view text) {
    auto copy = wstring(text);
    SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)copy.c_str());
}

void EditControl::SetNumber(unsigned int value) {
    static wchar_t szNumber[11]; //  4294967295
    String::Printf(szNumber, L"%u", value);
    SetText(szNumber);
}

unsigned int EditControl::GetNumber() const {
    unsigned int value = 0;
    auto text = GetText();
    swscanf(text.c_str(), L"%u", &value);
    return value;
}

void EditControl::SetByte(byte value) {
    static wchar_t szByte[3];
    String::Printf(szByte, L"%02hX", value);
    SetText(szByte);
}


byte EditControl::GetByte() const {
    uint16_t value = 0;
    auto text = GetText();
    swscanf(text.c_str(), L"%02hX", &value);  // Word, because sscanf cannot handle bytes correctly
    return (byte)value;
}

void EditControl::SetAddress(Memory::address address) {
    SetTextSize(4);
    static wchar_t szAddress[5];
    String::Printf(szAddress, L"%04hX", address);
    SetText(szAddress);
}

Memory::address EditControl::GetAddress() const {
    Memory::address address = 0;
    auto text = GetText();
    swscanf(text.c_str(), L"%04hX", &address);
    return address;
}

wstring EditControl::GetText() const {
    static wchar_t buffer[1024];
    strclr(buffer);
    SendMessage(hWnd, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
    return wstring(buffer);
}

bool EditControl::HasText() const {
    return GetTextLength() > 0;
}

int EditControl::GetTextLength() const {
    int result = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
    return result;
}
