#include "DisassemblyResultFile.h"

#include "Application.h"
#include "DisassemblyResult.h"
#include "DisassemblyLine.h"
#include "DisassemblySectionType.h"
#include "DisassemblyResultWriter.h"
#include "Profile.h"
#include "FileIO.h"
#include "File.h"
#include "String.h"
#ifdef _WIN32
#include <Windows.h>
#endif

extern std::unique_ptr<Application> g_Application;

DisassemblyResultFile::DisassemblyResultFile() : result(nullptr), profile(nullptr) {
}

wstring DisassemblyResultFile::CreateIncludeFilePath(wstring_view mainFilePath, FileNumber fileNumber) {

    wstring result;

    if (fileNumber > 999) {
        throw std::runtime_error("File number exceeds 3 digits");
    }

    result = mainFilePath;
    const auto index = result.find_last_of('.');
    if (index != wstring::npos) {
        result = result.substr(0, index);
    }

    if (fileNumber > 0) {
        wstring buffer = L".000";
        wsprintf(buffer.data(), L".%03u", fileNumber);
        result.append(buffer);
    }
    else {
        result.append(L".inc");
    }

    return result;
}

File DisassemblyResultFile::CreateIncludeFile(File mainFile, FileNumber fileNumber) {
    return File(CreateIncludeFilePath(mainFile.GetPath(), fileNumber));
}

void DisassemblyResultFile::OpenWriter(DisassemblyResultWriter& writer, File file) {
    ::g_Application->SendInfoMessageWithID(IDS_LOG_SAVE_DISASSEMBLY, file.GetPath());
    writer.OpenFile(file);
}

void DisassemblyResultFile::InsertDisLine(DisassemblyResultWriter& writer, const DIS_LINE& disLine) {
    if ((disLine.GetSection()->GetType() != DisassemblySectionType::SYSTEM_EQUATES) ||
        (disLine.systemAddress == 0) ||
        (disLine.referenced) ||
        (!profile->omitUnreferencedSystemLabels)) {
        writer.PrintLine(disLine.GetLine(), false);
    }
}

void DisassemblyResultFile::SaveListingWithoutIncludes(DisassemblyResultWriter& mainWriter, wstring_view atariFileName) {

    for (auto i = result->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        InsertDisLine(mainWriter, *disLine);
    }

    mainWriter.InsertDirectiveEnd(atariFileName);
}

void DisassemblyResultFile::SaveListingWithOneInclude(DisassemblyResultWriter& mainWriter, wstring_view atariFileName) {

    auto includeWriter = DisassemblyResultWriter(*profile);
    OpenWriter(includeWriter, CreateIncludeFile(mainWriter.GetFile()));
    for (auto i = result->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        if (disLine->GetSection()->GetType() != DisassemblySectionType::CODE_LINES) {
            InsertDisLine(includeWriter, *disLine);
        }
    }

    includeWriter.Close();

    mainWriter.InsertComment();
    mainWriter.InsertIncludeFileStatement(CreateIncludeFilePath(atariFileName, 0));

    for (auto i = result->CreateConstLineIterator(DisassemblySectionType::CODE_LINES); i->HasNext();) {
        mainWriter.PrintLine(i->Next()->GetLine(), false);
    }
    mainWriter.InsertDirectiveEnd(atariFileName);
}

void DisassemblyResultFile::SaveListingWithIncludesInMainFile(DisassemblyResultWriter& mainWriter, wstring_view atariFileName, LineCount maximumNumberOfLinesPerFile) {

    auto includeWriter = DisassemblyResultWriter(*profile);

    FileNumber fileNumber = 1;
    includeWriter.OpenFile(CreateIncludeFile(mainWriter.GetFile(), fileNumber));

    for (auto i = result->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        InsertDisLine(includeWriter, *disLine);

        if (includeWriter.GetLineNumber() == maximumNumberOfLinesPerFile && i->HasNext()) {
            fileNumber++;
            includeWriter.Close();
            includeWriter.OpenFile(CreateIncludeFile(mainWriter.GetFile(), fileNumber));
        }
    }
    includeWriter.Close();

    mainWriter.InsertComment();
    mainWriter.InsertComment(atariFileName);
    mainWriter.InsertComment();
    for (LineNumber i = 1; i <= fileNumber; i++) {
        mainWriter.InsertIncludeFileStatement(CreateIncludeFilePath(atariFileName, i));
    }

    mainWriter.InsertDirectiveEnd(atariFileName);
}
void DisassemblyResultFile::SaveListingWithIncludeInEachFile(DisassemblyResultWriter& mainWriter, wstring_view atariFileName, LineCount maximumNumberOfLinesPerFile) {

    auto includeWriter = DisassemblyResultWriter(*profile);
    FileNumber fileNumber = 0;

    auto writer = &mainWriter;

    for (auto i = result->CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();

        InsertDisLine(*writer, *disLine);

        if (writer->GetLineNumber() == maximumNumberOfLinesPerFile && i->HasNext()) {
            fileNumber++;

            writer->InsertComment();
            writer->InsertIncludeFileStatement(CreateIncludeFilePath(atariFileName, fileNumber));

            writer = &includeWriter;
            writer->OpenFile(CreateIncludeFile(mainWriter.GetFile(), fileNumber));
        }
    }

    writer->InsertDirectiveEnd(atariFileName);
    includeWriter.Close();
}

void DisassemblyResultFile::SaveListing(DisassemblyResultWriter& mainWriter, wstring_view atariFileName) {
    if (profile->directiveINCLUDEAllowed == false) {
        SaveListingWithoutIncludes(mainWriter, atariFileName);
    }
    else if (profile->directiveINCLUDEAllEquatesInOneIncludeFile) {
        SaveListingWithOneInclude(mainWriter, atariFileName);
    }
    else {

        // A limit of zero means no limit.
        LineCount maximumNumberOfLinesPerFile = profile->directiveINCLUDEMaximumNumberOfLinesPerFile;
        if (maximumNumberOfLinesPerFile == 0) {
            maximumNumberOfLinesPerFile = ULONG_MAX;
        }
        if (profile->directiveINCLUDEAllIncludesInMainFile) {
            SaveListingWithIncludesInMainFile(mainWriter, atariFileName, maximumNumberOfLinesPerFile);
        }
        else {
            SaveListingWithIncludeInEachFile(mainWriter, atariFileName, maximumNumberOfLinesPerFile);
        }
    }
}

void DisassemblyResultFile::SaveListing(const DisassemblyResult& result, const Profile& profile, File mainFile) {
    this->result = &result;
    this->profile = &profile;

    auto mainWriter = DisassemblyResultWriter(profile);
    OpenWriter(mainWriter, mainFile);
    try {
        auto atariFileName = mainFile.GetName();
        SaveListing(mainWriter, atariFileName);
    }
    catch (const IOException& ex) {
        mainWriter.Close();
        throw ex;
    }
    mainWriter.Close();
}