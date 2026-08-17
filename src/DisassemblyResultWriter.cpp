#include "DisassemblyResultWriter.h"

#include "Encoding.h"
#include "File.h"
#include "FileIO.h"
#include "OutputStream.h"
#include "Strings.h"

DisassemblyResultWriter::DisassemblyResultWriter(const Profile& profile) :
    profile(&profile), file(), outputStream(nullptr), lineNumber(0) {
}

void DisassemblyResultWriter::OpenFile(File file) {

    this->file = file;
    auto encoding = profile->outputEncoding;
    outputStream = OutputStream::OpenFile(this->file.GetPath(), encoding);
    newline = EncodingFactory::GetInfo(encoding).newline;
    lineNumber = 0;
}

void DisassemblyResultWriter::Close() {
    if (outputStream != nullptr) {
        delete outputStream;
    }
}

File DisassemblyResultWriter::GetFile() const {
    return file;
}

DisassemblyResultWriter::LineNumber DisassemblyResultWriter::GetLineNumber() const {
    return lineNumber;
}

void DisassemblyResultWriter::PrintLine(wstring_view line, bool useAlignment) {
    lineNumber++;

    if (outputStream == nullptr) {
        throw std::runtime_error("No output stream opened");
    }
    wstring lineNumberString;
    if (profile->useLineNumbers) {
        lineNumberString.append(std::to_wstring(lineNumber)).append(L" ");
        outputStream->WriteString(lineNumberString);
    }

    if (useAlignment && !line.starts_with(profile->commentPrefix)) { // TODO Alignment disabled, because comments and labels cannot be aligned
        const auto lineNumberStringLength = lineNumberString.length();
        size_t instructionAlignmentOffset = (profile->alignInstructions ? 8 : 0);
        for (size_t i = lineNumberStringLength; i < instructionAlignmentOffset; i++) {
            outputStream->WriteString(L" ");
        }
    }
    outputStream->WriteString(line);
    outputStream->WriteString(newline);
}


void DisassemblyResultWriter::InsertComment(wstring_view comment) {

    if (profile->commentPrefix.empty()) {
        return;

    }
    auto line = profile->commentPrefix;
    if (comment.size() > 0) {
        line.append(L" ");
        line.append(comment);
    }
    PrintLine(line, false);

}

void DisassemblyResultWriter::InsertIncludeFileStatement(wstring_view includeFileName) {
    auto line = profile->directiveINCLUDEHead;
    line.append(includeFileName);
    line.append(profile->directiveINCLUDETail);

    PrintLine(line, true);
}


void DisassemblyResultWriter::InsertDirectiveEnd(wstring_view atariFileName) {
    if (profile->directiveENDHead.empty()) {
        return;
    }
    InsertComment();

    auto line = profile->directiveENDHead;
    if (profile->directiveENDNeedsFilename) {
        line.append(atariFileName);
    }
    line.append(profile->directiveENDTail);
    PrintLine(line, true);
}

