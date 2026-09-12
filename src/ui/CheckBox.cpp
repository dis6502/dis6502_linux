#include "CheckBox.h"
#include "Control.h"
#include "Window.h"
#include "Windowsx.h" // For Button_Set/GetCheck
#include <Windows.h>


CheckBox::CheckBox(WindowHandle hWnd) : Control(hWnd) {}


void CheckBox::SetChecked(bool checked) {
    Button_SetCheck(hWnd, checked ? TRUE : FALSE);
}

bool CheckBox::IsChecked() const {
    if (Button_GetCheck(hWnd) == TRUE) {
        return true;
    }
    return false;
}
