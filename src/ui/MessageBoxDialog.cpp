#include "MessageBoxDialog.h"
#include "UI.h"

MessageBoxDialog::ButtonID  MessageBoxDialog::Show(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType) {
#ifdef _WIN32
    auto hWnd = parentWindow != nullptr ? parentWindow->GetHWnd() : NULL_HWND;
    return (ButtonID)MessageBox(hWnd, wstring(message).c_str(), wstring(title).c_str(), (UINT)buttonType); // #.c_str() OK
#else
    ShowMessage(title, message);
    return ButtonID::OK;
#endif
}

MessageBoxDialog::ButtonID  MessageBoxDialog::ShowAlert(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType) {
#ifdef _WIN32
    auto hWnd = parentWindow != nullptr ? parentWindow->GetHWnd() : NULL_HWND;
    return (ButtonID)MessageBox(hWnd, wstring(message).c_str(), wstring(title).c_str(), MB_ICONEXCLAMATION | (UINT)buttonType); // #.c_str() OK
#else
    ShowMessage(title, message);
    return ButtonID::OK;
#endif
}

MessageBoxDialog::ButtonID MessageBoxDialog::ShowQuestion(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType) {
#ifdef _WIN32
    auto hWnd = parentWindow != nullptr ? parentWindow->GetHWnd() : NULL_HWND;
    return (ButtonID)MessageBox(hWnd, wstring(message).c_str(), wstring(title).c_str(), MB_ICONQUESTION | (UINT)buttonType); // #.c_str() OK
#else
    ShowMessage(title, message);
    return ButtonID::OK;
#endif
}

void ShowMessage(wstring_view title, wstring_view message) {
    std::fwprintf(stderr, L"[dialog] %ls: %ls\n", wstring(title).c_str(), wstring(message).c_str());
}

