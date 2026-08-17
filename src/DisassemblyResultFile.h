#pragma once

#include "Syntax.h"
#include "Memory.h"
#include "File.h"

class DisassemblyResult;
class Profile;
class DisassemblyResultFileTest;
class DisassemblyResultConstLineIterator;
class DIS_LINE;
class DisassemblyResultWriter;

class DisassemblyResultFile {
public:
    DisassemblyResultFile();

    DisassemblyResultFile(const DisassemblyResultFile& a) = delete;			  // copy constructor
    DisassemblyResultFile& operator=(const DisassemblyResultFile&) = delete;  // copy assignment
    DisassemblyResultFile(DisassemblyResultFile&&) = delete;                  // move constructor
    DisassemblyResultFile& operator=(DisassemblyResultFile&&) = delete;       // move assignment

    void SaveListing(const DisassemblyResult& result, const Profile& profile, File mainFile); // throws IOException // todo return include files

private:
    friend DisassemblyResultFileTest;
    typedef unsigned int FileNumber;
    typedef unsigned long LineCount;
    typedef unsigned long LineNumber;

    const DisassemblyResult* result;
    const Profile* profile;

    /*
    ** fileNumber >= 0 to have the number appended, 0 to have a single include file.
    */
    static wstring CreateIncludeFilePath(wstring_view mainFilePath, FileNumber fileNumber=0);
    static File CreateIncludeFile(File mainFile, FileNumber fileNumber = 0);

    void OpenWriter(DisassemblyResultWriter& writer, File file);
    void InsertDisLine(DisassemblyResultWriter& writer, const DIS_LINE& disLine);

    void SaveListingWithoutIncludes(DisassemblyResultWriter& mainWriter, wstring_view atariFileName);
    void SaveListingWithOneInclude(DisassemblyResultWriter& mainWriter, wstring_view atariFileName);
    void SaveListingWithIncludesInMainFile(DisassemblyResultWriter& mainWriter, wstring_view atariFileName, LineCount maximumNumberOfLinesPerFile);
    void SaveListingWithIncludeInEachFile(DisassemblyResultWriter& mainWriter, wstring_view atariFileName, LineCount maximumNumberOfLinesPerFile);

    void SaveListing(DisassemblyResultWriter& mainWriter, wstring_view watariFileName);
};
