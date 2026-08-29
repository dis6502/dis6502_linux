#pragma once

#include "Syntax.h"
#include "UI.h"
#include "Control.h"
#include "Text.h"
#include <vector>

class ListBox : public Control {

public:
    ListBox(HWND hWnd);

    void Clear();

    // The return value is the zero - based index of the string in the list box.
    // If an error occurs, the return value is LB_ERR.
    // If there is insufficient space to store the new string, the return value is LB_ERRSPACE.
    int AddString(wstring_view text, void* object = nullptr);
    int AddString(Text::TextID textID, void* object = nullptr);
    void DeleteStringAtIndex(int index);
    int GetCount() const;
    wstring GetStringAtIndex(int index) const;
    void* GetObjectAtIndex(int index) const;

    bool IsSelectedIndex(int index) const;
    // In a single-selection list box, the return value is the zero-based index of the currently selected item.
    // If there is no selection, the return value is LB_ERR.
    int GetSelectedIndex() const;
    void SetSelectedIndex(int index);

    void* GetSelectedObject() const;
    void SetSelectedObject(const void* object);
};
