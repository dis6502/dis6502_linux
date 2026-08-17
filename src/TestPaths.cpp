#include "TestPaths.h"
#include "Application.h"

wstring TestPaths::GetSuitePath() {
    return g_Application->GetModuleFilePath(L"../../../tst/suite");
}
