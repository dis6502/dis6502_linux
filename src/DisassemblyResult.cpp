#include "DisassemblyResult.h"

#include "DisassemblyLine.h"

std::vector<DisassemblySectionType> DisassemblyResult::sectionTypes =
{ DisassemblySectionType::SYSTEM_EQUATES, DisassemblySectionType::USER_EQUATES, DisassemblySectionType::CODE_EQUATES, DisassemblySectionType::CODE_LINES };


DisassemblyResult::DisassemblyResult() noexcept {

    sections.resize(sectionTypes.size());

}

const std::vector<DisassemblySectionType>& DisassemblyResult::GetSectionTypes() const {
    return sectionTypes;
}

std::unique_ptr<DisassemblyResultLineIterator> DisassemblyResult::CreateLineIterator() {
    return std::make_unique<DisassemblyResultLineIterator>(*this, 0, GetSectionCount() - 1, GetLineCount());
}

std::unique_ptr<DisassemblyResultLineIterator> DisassemblyResult::CreateLineIterator(DisassemblySectionType disassemblySectionType) {
    const auto index = DisassemblySection::GetIndex(disassemblySectionType);
    LineCount lineCount = 0;
    const DisassemblySection* section = GetSection(disassemblySectionType);
    if (section) {
        lineCount = section->GetLineCount();
    }
    return std::make_unique<DisassemblyResultLineIterator>(*this, index, index, lineCount);
}


std::unique_ptr<DisassemblyResultConstLineIterator> DisassemblyResult::CreateConstLineIterator() const {
    return std::make_unique<DisassemblyResultConstLineIterator>(*this, 0, GetSectionCount() - 1, GetLineCount());
}

std::unique_ptr<DisassemblyResultConstLineIterator> DisassemblyResult::CreateConstLineIterator(DisassemblySectionType disassemblySectionType) const {
    const auto index = DisassemblySection::GetIndex(disassemblySectionType);
    LineCount lineCount = 0;
    auto section = GetConstSection(index);
    if (section) {
        lineCount += section->GetLineCount();
    }
    return std::make_unique<DisassemblyResultConstLineIterator>(*this, index, index, lineCount);
}


void DisassemblyResult::Clear() {
    for (size_t i = 0; i < GetSectionCount(); i++) {
        sections.at(i).reset();
    }
}

size_t DisassemblyResult::GetSectionCount() const {
    return sections.size();
}

DisassemblySection* DisassemblyResult::GetSection(size_t index) {
    return sections.at(index).get();
}

const DisassemblySection* DisassemblyResult::GetConstSection(size_t index) const {
    return sections.at(index).get();
}

DisassemblySection* DisassemblyResult::GetSection(DisassemblySectionType disassemblySectionType) {
    return GetSection(DisassemblySection::GetIndex(disassemblySectionType));
}

DisassemblyResult::LineCount DisassemblyResult::GetLineCount() const {
    LineCount lineCount = 0;;
    for (size_t i = 0; i < sections.size(); i++) {
        auto section = GetConstSection(i);
        if (section) {
            lineCount += section->GetLineCount();
        }
    }
    return lineCount;
}

DIS_LINE* DisassemblyResult::SelectLine(LineNumber lineNumber) {
    DIS_LINE* selectedDisLine = nullptr;

    for (auto i = CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        if (disLine->GetLineNumber() == lineNumber) {
            selectedDisLine = disLine;
            disLine->selected = true;
        }
        else {
            disLine->selected = false;
        }
    }
    return selectedDisLine;
}

DisassemblyResult::LineNumber DisassemblyResult::SelectLine(SEGMENT_INDEX segmentIndex, Memory::offset offset) {

    LineNumber selectedLineNumber = 0;
    for (auto i = CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        if ((selectedLineNumber == 0) && (disLine->segmentIndex == segmentIndex) && (disLine->offset == offset)) {
            selectedLineNumber = disLine->GetLineNumber();
            disLine->selected = true;
        }
        else {
            disLine->selected = false;
        }
    }

    return selectedLineNumber;
}

bool DisassemblyResult::ExtendSelectionTo(SEGMENT_INDEX segmentIndex, Memory::offset offset) {
    DisassemblyResult::LineNumber selectedLineNumber = 0;
    for (auto i = CreateConstLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();
        if ((disLine->segmentIndex == segmentIndex) && (disLine->offset == offset)) {
            selectedLineNumber = disLine->GetLineNumber();
        }
    }

    if (selectedLineNumber != 0) {
        bool selected = false;
        for (auto i = CreateLineIterator(); i->HasNext();) {
            const auto& disLine = i->Next();
            if (disLine->GetLineNumber() > selectedLineNumber) {
                break; // for()
            }
            if (disLine->selected) {
                selected = true;
            }
            disLine->selected = selected;
        }
        return true;
    }
    return false;
}


/*
** Allocate a buffer and add it at the end of a list.
*/
gsl::not_null<DisassemblySection*> DisassemblyResult::AllocSection(DisassemblySectionType disassemblySectionType) {
    // allocate a new section and initialize variables.
    const auto sectionIndex = DisassemblySection::GetIndex(disassemblySectionType);
    sections.at(sectionIndex) = std::make_unique<DisassemblySection>(*this, disassemblySectionType);

    return sections.at(sectionIndex).get();
}

/*
** Clear buffer, i.e. free all the buffers in a list for a section type.
*/
void DisassemblyResult::ClearSection(DisassemblySectionType disassemblySectionType) {
    const auto i = DisassemblySection::GetIndex(disassemblySectionType);
    sections.at(i).reset();
}

void DisassemblyResult::ClearEquateSections() {
    ClearSection(DisassemblySectionType::SYSTEM_EQUATES);
    ClearSection(DisassemblySectionType::USER_EQUATES);
}

bool DisassemblyResult::FindAndSelectLines(bool first, DisassemblyResult::LineNumber& findFirstLineNumber, wstring_view findString) {
    wstring findStringString(findString);
    bool found = false;
    XRef::LineNumber xrefLineNumber = 1;

    for (auto i = CreateLineIterator(); i->HasNext();) {
        const auto& disLine = i->Next();

        // Reset selection flag on the current line.
        disLine->selected = false;

        if (first) {
            disLine->xrefLineNumber = 0;

            // Do we have the substring in the line ?
            if (wcsstr(disLine->GetLine(), findStringString.c_str())) {
                // This is the good line. Mark it as selected.
                disLine->xrefLineNumber = xrefLineNumber++;

                // Keep the first line for the next search operation.
                if (findFirstLineNumber == 0) {
                    disLine->selected = true;
                    findFirstLineNumber = disLine->GetLineNumber();
                }

                // Continue all the loop to reset other selection flags on other lines.
                found = true;
            }
        }
        else {
            // Ignore all the lines before the line found in a previous search and after the line has been found.
            if ((disLine->GetLineNumber() >= findFirstLineNumber) && (!found)) {
                // Do we have the substring in the line ?
                if (wcsstr(disLine->GetLine(), findStringString.c_str())) {
                    // This is the good line. Mark it as selected.
                    disLine->selected = true;
                    // Keep the first line for the next search operation.
                    findFirstLineNumber = disLine->GetLineNumber();
                    // Continue all the loop to reset other selection flags on other lines.
                    found = true;
                }
            }
        }
    }
    return found;
}

void DisassemblyResult::FindOffsetAtStartOfInstruction(SEGMENT_INDEX segmentIndex, Memory::offset& offset, Memory::size& size) const {
    offset = 0;
    size = 0;
    for (auto i = CreateConstLineIterator(DisassemblySectionType::CODE_LINES); i->HasNext();) {
        const auto& disLine = i->Next();

        if ((disLine->segmentIndex == segmentIndex)
            && (disLine->size > 0)
            && (disLine->offset <= offset)
            && ((disLine->offset + disLine->size) > offset)) {
            offset = disLine->offset;
            size = disLine->size;
            return;
        }

    }
}