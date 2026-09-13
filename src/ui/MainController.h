#pragma once

#include "Window.h"

class Application;
class Main;
class Workspace;
class SegmentList;
class MainWindow;


class MainController // abstract
{
public:
    using COMMAND = Window::COMMAND;

    ~MainController() = default;

    bool IsEditMode() const;
    void QuitEditMode();

protected:
    MainController(Main& main);

    Application* application;
    Main* main;
    MainWindow* mainWindow;

    void UpdateDisassembly(bool bForce = false);
};
