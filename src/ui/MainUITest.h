#pragma once

#include "Console.h"
#include "DefaultFoldersLogic.h"
#include "EquateListLogic.h"
#include "ProfileLogic.h"
#include "Syntax.h"
#include "systems/ComputerSystemFactory.h"
#include "TestUnit.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"

enum class FileType;
enum class ComputerSystemType;

class MainUITest {

public:
    MainUITest();
    bool Execute(wstring_view testModeName); // Returns true, if the main programm shall be started, too

private:

    Console console;
};

