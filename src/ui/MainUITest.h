#pragma once
#pragma once

#include "Console.h"
#include "Syntax.h"


enum class FileType;
enum class ComputerSystemType;

class MainUITest {

public:
    MainUITest();
    bool Execute(wstring_view testModeName); // Returns true, if the main programm shall be started, too

private:

    Console console;
};

