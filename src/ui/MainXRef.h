#pragma once

#include "Syntax.h"
#include <vector>
#include "MainController.h"

class MainDisassembly;

class MainXRef final : public MainController {
public:
    MainXRef(Main& main);
    void SetMainDisassembly(MainDisassembly& mainDisassembly);

    long Proc(HWND hWnd, WPARAM wParam);

    void UpdateList(wstring_view findStrin, std::vector<wstring> references);

private:
    MainDisassembly* mainDisassembly;

    int updateCounter;

    void HandleSelectionChanged();

};
