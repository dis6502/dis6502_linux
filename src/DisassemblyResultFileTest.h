#pragma once

#include "Syntax.h"
#include "File.h"
#include "DisassemblyResult.h"
#include "Profile.h"

class DisassemblyResultFileTest {
public:

    static void TestDisassemblyResultFile(File folder);

private:
    static void TestSaveListing(File folder, const DisassemblyResult& result, const Profile& profile, wstring_view variant);

};