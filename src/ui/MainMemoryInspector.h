#pragma once

#include "MainController.h"
#include "Window.h"
#include <Windows.h>

class MainMemoryInspector final : public MainController {
public:
    MainMemoryInspector(Main& main);

    long Proc(HWND hWnd, Window::WPARAM wParam);
    void PerformCommands(Window::COMMAND command, bool editMode);

private:
    void SelectAll();
    void SelectNextUnknownBlock();
    void Find();
    void FindNext();
    void Edit();

    void SaveWithoutHeader(const Window& parentWindow);
    void SaveWithHeader(const Window& parentWindow);
};
