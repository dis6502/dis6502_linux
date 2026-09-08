#include "Application.h"

#include "CheckBox.h"
#include "Windowsx.h" // For Button_Set/GetCheck

extern Application* g_Application;

CheckBox::CheckBox(HWND hWnd) : Control(hWnd) {}


void CheckBox::SetChecked(bool checked) {
    Button_SetCheck(hWnd, checked ? TRUE : FALSE);
}

bool CheckBox::IsChecked() const {
    if (Button_GetCheck(hWnd) == TRUE) {
        return true;
    }
    return false;
}
