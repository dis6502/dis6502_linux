#pragma once

#include "Syntax.h"
#include "PartWindow.h"
#include "Window.h"


class LogListWindow final : public PartWindow {
public:
    LogListWindow(Window& parentWindow, PartLayout& partLayout);
    ~LogListWindow() = default;

    void CreateControl(ChildID childID);

    void AddText(wstring_view text);
};
