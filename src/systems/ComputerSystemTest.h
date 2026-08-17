#pragma once

#include "Syntax.h"
#include "TestUnit.h"

class ComputerSystemFactory;
class ComputerSystem;

class ComputerSystemTest {

public:
    void TestSystems(const ComputerSystemFactory& computerSystemFactory);

private:
    TestUnit testUnit;
    void SetupUnit(wstring_view areaName, wstring_view unitName);

    void TestAtari800(const ComputerSystem& computerSystem);
    void TestC64(const ComputerSystem& computerSystem);

    void TestReadFile(const ComputerSystem& computerSystem, wstring_view fileNamme);
    void TestReadDiskImageFile(const ComputerSystem& computerSystem, wstring_view diskImageFilePath, wstring_view fileName, bool expectReadFailure);
};
