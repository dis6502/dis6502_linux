#pragma once

#include "Syntax.h"
#include <gsl/gsl>
#include "File.h"
#include "Profile.h"

class OutputStream;

class DisassemblyResultWriter {
public:


    typedef unsigned long LineNumber;

    DisassemblyResultWriter(const Profile& profile);
    ~DisassemblyResultWriter() = default;

    DisassemblyResultWriter(const DisassemblyResultWriter&) = delete;               // copy constructor		
    DisassemblyResultWriter& operator=(const DisassemblyResultWriter&) = delete;    // copy assignment
    DisassemblyResultWriter(DisassemblyResultWriter&&) = delete;                    // move constructor
    DisassemblyResultWriter& operator=(DisassemblyResultWriter&&) = delete;         // move assignment

    void OpenFile(File file); // throws IOException
    File GetFile() const;
    void Close();

    LineNumber GetLineNumber() const;
    void PrintLine(wstring_view line, bool useAlignment);


    void InsertComment(wstring_view comment = L"");
    void InsertIncludeFileStatement(wstring_view  includeFileName);
    void InsertDirectiveEnd(wstring_view atariFileName);

private:

    const Profile* profile;

    File file;
    OutputStream* outputStream;
    wstring newline;
    LineNumber lineNumber;

};
