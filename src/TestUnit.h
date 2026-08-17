#pragma once

#include "Syntax.h"

#include "File.h"

class TestUnit {

public:
    TestUnit() = default;
    TestUnit(wstring_view basePath, wstring_view areaName, wstring_view unitName);

    wstring GetInFilePath(wstring_view fileName) const;
    wstring GetRefFilePath(wstring_view fileName) const;
    wstring GetOutFilePath(wstring_view fileName) const;
    File GetOutFile(wstring_view fileName) const;

private:
    wstring inPath;
    wstring refPath;
    wstring outPath;
    File outFolder;
};