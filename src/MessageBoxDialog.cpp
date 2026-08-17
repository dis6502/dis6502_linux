#include "MessageBoxDialog.h"
#include "PlatformCompat.h"

#include <cstdio>

int MessageBoxDialog::Show(void* parentWindow, wstring_view title, wstring_view message, unsigned int uType) {
    (void)parentWindow;
    (void)uType;
    std::fwprintf(stderr, L"[dialog] %ls: %ls\n", wstring(title).c_str(), wstring(message).c_str());
    return 0; // IDOK
}
