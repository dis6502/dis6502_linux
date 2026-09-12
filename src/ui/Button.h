#pragma once
#include "Control.h"

class Button : public Control {
public:
    Button(WindowHandle hWnd);

    Button() = delete;
    Button(const Button& a) = delete;			// copy constructor
    Button& operator=(const Button&) = delete;  // copy assignment
    Button(Button&&) = delete;                  // move constructor
    Button& operator=(Button&&) = delete;       // move assignment
};
