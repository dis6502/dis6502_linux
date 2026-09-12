#include "ComboBox.h"
#include "Dialog.h"
#include "Syntax.h"
#include "Text.h"
#include <stdexcept>

ComboBox::ComboBox(HWND hWnd) : Control(hWnd) {}

void ComboBox::Clear() {
    SendMessage(hWnd, CB_RESETCONTENT, 0, 0);
}

void ComboBox::AddString(wstring_view text, void* object) {
    wstring entry = wstring(text);
    const auto index = (int)SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)entry.c_str()); // #.c_str() OK
    if (index == CB_ERR || index == CB_ERRSPACE) {
        throw std::runtime_error("Cannot add combobox item");
    }
    if (object != nullptr) {
        SendMessage(hWnd, CB_SETITEMDATA, index, (LPARAM)object);
    }
}

int ComboBox::GetCount() const {
    return SendMessage(hWnd, CB_GETCOUNT, 0, (LPARAM)0);
}

void ComboBox::AddString(Text::TextID textID, void* object) {
    auto string = Text::Get(textID);
    AddString(string, object);
}

int ComboBox::GetSelectedIndex() const {
    return (int)SendMessage(hWnd, CB_GETCURSEL, 0, 0);
}

void* ComboBox::GetSelectedObject() const {
    void* object = nullptr;

    const auto index = GetSelectedIndex();
    if (index >= 0) {
        object = (void*)SendMessage(hWnd, CB_GETITEMDATA, index, 0);
    }

    return object;
}

void ComboBox::SetSelectedIndex(int index) {
    SendMessage(hWnd, CB_SETCURSEL, (WPARAM)index, 0);
}

void ComboBox::SetSelectedString(wstring_view text) {
    auto string = wstring(text);
    SendMessage(hWnd, CB_SELECTSTRING, 0, (LPARAM)string.c_str());
}
