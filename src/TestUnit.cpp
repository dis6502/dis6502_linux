#include "TestUnit.h"

#include "FileIO.h"
#include "File.h"

TestUnit::TestUnit(wstring_view basePath, wstring_view areaName, wstring_view unitName): TestUnit() {
    auto areaPath = wstring(basePath) + FileIO::FILE_SEPARATOR + wstring(areaName);
    auto unitPath = areaPath + FileIO::FILE_SEPARATOR + wstring(unitName);
    inPath = unitPath + FileIO::FILE_SEPARATOR + L"in";
    refPath = unitPath + FileIO::FILE_SEPARATOR + L"ref";
    outPath = unitPath + FileIO::FILE_SEPARATOR + L"out";
    outFolder = File(outPath);
}

wstring TestUnit::GetInFilePath(wstring_view fileName) const {
    return inPath + FileIO::FILE_SEPARATOR + wstring(fileName);
}

wstring TestUnit::GetRefFilePath(wstring_view fileName) const {
    return refPath + FileIO::FILE_SEPARATOR + wstring(fileName);
}
wstring TestUnit::GetOutFilePath(wstring_view fileName) const {
    return outPath + FileIO::FILE_SEPARATOR + wstring(fileName);
}

File TestUnit::GetOutFile(wstring_view fileName) const {
    return File(outFolder ,fileName);
}