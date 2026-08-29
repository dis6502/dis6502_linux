#pragma once

#include "Syntax.h"
#include "UI.h"
#include "PartWindow.h"


class LogListWindow final : public PartWindow {
public:
    LogListWindow(Window& parentWindow, PartLayout& partLayout);
    ~LogListWindow() = default;

    void CreateControl(ChildID childID);

    void AddText(wstring_view text);
};
