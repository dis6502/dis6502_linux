#pragma once

#include "MainController.h"
#include "Syntax.h"
#include <vector>

class MainDisassembly;

class MainXRef final : public MainController {
public:
    MainXRef(Main& main);
    void SetMainDisassembly(MainDisassembly& mainDisassembly);

    long Proc(Window::WindowHandle hWnd, Window::WPARAM wParam);

    void UpdateList(wstring_view findStrin, std::vector<wstring> references);

private:
    MainDisassembly* mainDisassembly;

    int updateCounter;

    void HandleSelectionChanged();

};
