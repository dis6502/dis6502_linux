#pragma once

#include "Syntax.h"
#include "Window.h"
#include <gsl/pointers>

class PartLayout;


class PartWindow : public Window {
public:
    PartWindow(Window& parentWindow, PartLayout& partLayout);

    wstring GetTitle() const;
    void SetTitle(wstring_view title);

    virtual void ApplyLayout();

protected:
    gsl::not_null<PartLayout*> partLayout;
    wstring title;

    void CreateControl(wstring_view className, WindowStyle style, ChildID childID);
};
