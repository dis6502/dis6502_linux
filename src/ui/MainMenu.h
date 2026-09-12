#pragma once


#include "MainController.h"
#include "UI.h"

class MRUController;


class MainMenu final : public MainController {
public:
    MainMenu(Main& main, MRUController& mruController);
    void PerformCommands(COMMAND command, bool editMode);

private:
    MRUController* mruController;

    void PerformFileMenuCommands(COMMAND command, bool editMode);
    void PerformLabelsMenuCommands(COMMAND command);
    void PerformViewMenuCommands(COMMAND command);
    void PerformHelpMenuCommands(COMMAND command);

};
