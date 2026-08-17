#include "DisassemblyResultFile.h"
#include "DisassemblyResultFileTest.h"
#include "DisassemblyResultTest.h"

#include "Assertions.h"
#include "FileIO.h"
#include "Strings.h"

void DisassemblyResultFileTest::TestDisassemblyResultFile(File folder) {

    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example"), L"example.inc");
    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example."), L"example.inc");
    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example.ext"), L"example.inc");

    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example", 1), L"example.001");
    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example.", 2), L"example.002");
    Assert::StringEquals(DisassemblyResultFile::CreateIncludeFilePath(L"example.ext", 3), L"example.003");

    DisassemblyResult result;
    DisassemblyResultTest::GenerateDisassemblyResult(result, 10);

    Profile profile;
    profile.omitUnreferencedSystemLabels = false;

    profile.directiveINCLUDEAllowed = false;
    TestSaveListing(folder, result, profile, L"1-noinc");

    profile.directiveINCLUDEAllowed = true;
    profile.directiveINCLUDEAllEquatesInOneIncludeFile = true;
    TestSaveListing(folder, result, profile, L"2-oneinc");

    profile.directiveINCLUDEAllEquatesInOneIncludeFile = false;
    profile.directiveINCLUDEMaximumNumberOfLinesPerFile = 10;

    profile.directiveINCLUDEAllIncludesInMainFile = true;
    TestSaveListing(folder, result, profile, L"3-maininc");

    profile.directiveINCLUDEAllEquatesInOneIncludeFile = false;
    profile.directiveINCLUDEAllIncludesInMainFile = false;
    TestSaveListing(folder, result, profile, L"4-chaininc");
}


void  DisassemblyResultFileTest::TestSaveListing(File folder, const DisassemblyResult& result, const Profile& profile, wstring_view variant) {
    auto variantFolder = File(folder, variant);
    auto file = File(variantFolder, L"DisassemblyResultFileTest.asm");
    Assert::Log(String::Format(L"Saving disassembly result to {0}", file.GetPath()));
    DisassemblyResultFile disassemblyResultFile = DisassemblyResultFile();
    disassemblyResultFile.SaveListing(result, profile, file);

}