#include "MainTest.h"
#include "MainUITest.h"
#include "Syntax.h"

using TestMode = MainTest::TestMode;

MainUITest::MainUITest() {

}

bool MainUITest::Execute(wstring_view testModeName) {
    auto testMode = TestModeFromString(testModeName);

    if (testMode != TestMode::UNDEFINED) {
        console.Allocate();
        MainTest mainTest(console);
        mainTest.Execute(testMode);
    }
    else {
        console.Allocate();
        console.Write(L"ERROR: Unknown test mode {0}.", testModeName);
    }

    if (testMode == TestMode::DEV) {
        return true;
    }
    else {
        console.Allocate();
        console.ReadLine();
        return false;
    }
}
