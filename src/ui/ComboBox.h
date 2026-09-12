#pragma once

#include "Control.h"
#include "Syntax.h"
#include "Text.h"

class ComboBox : public Control {
public:
    ComboBox(WindowHandle hWnd);

    ComboBox() = delete;
    ComboBox(const ComboBox& a) = delete;			// copy constructor
    ComboBox& operator=(const ComboBox&) = delete;  // copy assignment
    ComboBox(ComboBox&&) = delete;                  // move constructor
    ComboBox& operator=(ComboBox&&) = delete;       // move assignment

    void Clear();
    void AddString(wstring_view text, void* object = nullptr);
    void AddString(Text::TextID textID, void* object = nullptr);
    int GetCount() const;

    int GetSelectedIndex() const;
    void* GetSelectedObject() const;
    void SetSelectedIndex(int index);
    void SetSelectedString(wstring_view text);
};
