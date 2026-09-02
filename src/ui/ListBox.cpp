#include "Dialog.h"
#include "ListBox.h"
#include "StringUtility.h"



ListBox::ListBox(HWND hWnd) : Control(hWnd) {}

void ListBox::Clear() {
    SendMessage(hWnd, LB_RESETCONTENT, 0, 0);
}

int ListBox::AddString(wstring_view text, void* object) {
    auto index = (int)(SendMessage(hWnd, LB_ADDSTRING, 0, (LPARAM)wstring(text).c_str()));
    if (index == LB_ERR || index == LB_ERRSPACE) {
        throw std::runtime_error("Cannot add list item");
    }
    if (object != nullptr) {
        SendMessage(hWnd, LB_SETITEMDATA, index, (LPARAM)object);

    }
    return index;
}

int ListBox::AddString(Text::TextID textID, void* object) {
    auto text = Text::Get(textID);

    return AddString(text, object);
}

void ListBox::DeleteStringAtIndex(int index) {
    SendMessage(hWnd, LB_DELETESTRING, index, 0);
}

int ListBox::GetCount() const {
    return SendMessage(hWnd, LB_GETCOUNT, 0, 0);
}

wstring ListBox::GetStringAtIndex(int index) const {
    return String::FromCString((wchar_t*)(SendMessage(hWnd, LB_GETTEXT, index, 0)));
}

void* ListBox::GetObjectAtIndex(int index) const {
    return (void*)SendMessage(hWnd, LB_GETITEMDATA, index, 0);

}

bool ListBox::IsSelectedIndex(int index) const {
    if (SendMessage(hWnd, LB_GETSEL, index, 0)) {
        return true;
    }
    return false;
}

int ListBox::GetSelectedIndex() const {
    return SendMessage(hWnd, LB_GETCURSEL, 0, 0);
}

void ListBox::SetSelectedIndex(int index) {
    SendMessage(hWnd, LB_SETCURSEL, (WPARAM)index, 0);
}

void* ListBox::GetSelectedObject() const {
    auto index = GetSelectedIndex();
    if (index >= 0) {
        return GetObjectAtIndex(index);
    }
    return nullptr;
}

void ListBox::SetSelectedObject(const void* object) {
    if (object == nullptr) {
        SetSelectedIndex(LB_ERR);
        return;
    }
    const auto count = GetCount();
    for (int i = 0; i < count; i++) {
        if (GetObjectAtIndex(i) == object) {
            SetSelectedIndex(i);
            return;
        }
    }
}