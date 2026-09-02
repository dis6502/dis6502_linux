

#include <cstdio>
#include <exception>

#include "MainTest.h"
#include "ui/Console.h"

int main() {
    Console console;
    MainTest mainTest(console);
    mainTest.Execute(MainTest::TestMode::FAST);

    return 0;
}
