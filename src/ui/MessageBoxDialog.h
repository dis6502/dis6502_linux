#pragma once
#include "Syntax.h"
#include "Window.h"

class MessageBoxDialog {
public:
    enum class ButtonType {
        OK = 0, // See MB_OK
        OKCANCEL = 1,
        ABORTRETRYIGNORE = 2,
        YESNOCANCEL = 3,
        YESNO = 4,
        RETRYCANCEL = 5,
        CANCELTRYCONTINUE = 6
    };

    enum class ButtonID {
        OK = 1, // See IDOK, etc.
        CANCEL = 2,
        ABORT = 3,
        RETRY = 4,
        /// IGNORE = 5, Already taken as #DEFINE
        YES = 6,
        NO = 7

};

    static ButtonID Show(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType = ButtonType::OK);
    static ButtonID ShowAlert(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType = ButtonType::OK);
    static ButtonID ShowQuestion(Window* parentWindow, wstring_view title, wstring_view message, ButtonType buttonType);
};

