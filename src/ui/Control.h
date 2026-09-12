#pragma once

#include "Font.h"
#include "Window.h"

class Control : public AbstractWindow {
public:

    using ScrollPosition = int;

    // Win32 callback requires __stdcall
    // TODO: Make private?
    static LRESULT __stdcall WndProc(WindowHandle hWnd, MESSAGE message, WPARAM wParam, LPARAM lParam);

    Control(WindowHandle hWnd);
    virtual ~Control();

    void SetEnabled(bool enabled);
    void SetFont(Font* font);

    bool HasFocus() const;
    void SetFocus();

    void SetScrollRange(int bar, ScrollPosition minPosition, ScrollPosition maxPosition, bool update);
    void SetScrollPosition(int bar, ScrollPosition scrollPosition, bool update);

    WindowHandle hWnd;

protected:
    virtual LRESULT WndProc(MESSAGE message, WPARAM wParam, LPARAM lParam);

private:
    static Control* GetInstance(WindowHandle hWND);

};
