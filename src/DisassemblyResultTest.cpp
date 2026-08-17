#include "DisassemblyBuffer.h"
#include "DisassemblyLine.h"
#include "DisassemblyResult.h"
#include "DisassemblyResultTest.h"

#include "DisassemblySection.h"
#include "DisassemblySectionType.h"
#include "Strings.h"

#include "Assertions.h"

void DisassemblyResultTest::TestDisassemblyResult() {
    DisassemblyResult result;

    Assert::LongEquals(result.GetSectionCount(), 4);
    Assert::LongEquals(result.GetLineCount(), 0);

    gsl::not_null<DisassemblySection*> section = result.AllocSection(DisassemblySectionType::SYSTEM_EQUATES);
    gsl::not_null<DIS_BUFFER*> buffer = section->AllocBuffer();
    auto disLine = DIS_LINE(*section);

    auto systemEquateline1 = L"; First System Equate";
    buffer->AddLine(disLine, systemEquateline1);
    Assert::LongEquals(result.GetLineCount(), 1);

    auto systemEquateline2 = L"; Second System Equate";
    buffer->AddLine(disLine, systemEquateline2);
    Assert::LongEquals(result.GetLineCount(), 2);

    section = result.AllocSection(DisassemblySectionType::USER_EQUATES);
    buffer = section->AllocBuffer();
    auto userEquateline1 = L"; First User Equate";
    buffer->AddLine(disLine, userEquateline1);
    Assert::LongEquals(result.GetLineCount(), 3);

    auto userEquateline2 = L"; Second User Equate";
    buffer->AddLine(disLine, userEquateline2);
    Assert::LongEquals(result.GetLineCount(), 4);

    auto resultIterator = result.CreateLineIterator();

    Assert::BoolEquals(resultIterator->HasNext(), true);
    gsl::not_null<DIS_LINE*> iteratorDisLine = resultIterator->Next();
    Assert::LongEquals(iteratorDisLine->GetLineNumber(), 1);
    Assert::StringEquals(iteratorDisLine->GetLine(), systemEquateline1);

    Assert::BoolEquals(resultIterator->HasNext(), true);
    iteratorDisLine = resultIterator->Next();
    Assert::LongEquals(iteratorDisLine->GetLineNumber(), 2);
    Assert::StringEquals(iteratorDisLine->GetLine(), systemEquateline2);

    Assert::BoolEquals(resultIterator->HasNext(), true);
    iteratorDisLine = resultIterator->Next();
    Assert::LongEquals(iteratorDisLine->GetLineNumber(), 3);
    Assert::StringEquals(iteratorDisLine->GetLine(), userEquateline1);

    Assert::BoolEquals(resultIterator->HasNext(), true);
    iteratorDisLine = resultIterator->Next();
    Assert::LongEquals(iteratorDisLine->GetLineNumber(), 4);
    Assert::StringEquals(iteratorDisLine->GetLine(), userEquateline2);

    Assert::BoolEquals(resultIterator->HasNext(), false);

    Assert::BoolEquals(true, true);
}

void DisassemblyResultTest::GenerateDisassemblyResult(DisassemblyResult& result, size_t linesPerSection) {

    result.Clear();
    DisassemblyResult::LineNumber lineNumber = 1;
    for (auto disassemblySectionType : result.GetSectionTypes()) {
        const auto& section = result.AllocSection(disassemblySectionType);
        const auto& buffer = section->AllocBuffer();
        wstring line;
        for (unsigned short int j = 0; (j < linesPerSection ) && (j < 0xc000 ); j++) {
            DIS_LINE disLine = DIS_LINE(*section);
            disLine.address = j;
            disLine.systemAddress = 0x1000 + j;
            line = String::Format(L"; Line number = {0}, section {1}, address {2} ", std::to_wstring(lineNumber), DisassemblySection::GetText(disassemblySectionType), std::to_hex_string(j));;
            buffer->AddLine(disLine, line);
            lineNumber++;
        }
    }
}
