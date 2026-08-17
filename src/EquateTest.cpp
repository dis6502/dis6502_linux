#include "EquateTest.h"
#include "Assertions.h"
#include "Equate.h"
#include "EquateType.h"
#include "LabelAccess.h"


wstring GetEquateTypeText(EquateType equateType) {
    return EquateTypeFactory::GetInfo(equateType).key;
}

void AssertEquateTypeEquals(EquateType actualEquateType, EquateType expectedEquateType) {
    if (actualEquateType != expectedEquateType) {
        Assert::Fail(wstringstream() << L"Actual equate type " << GetEquateTypeText(actualEquateType) << L" is different from expected equate " << GetEquateTypeText(expectedEquateType));
    }
}

const wchar_t* GetAccessText(LabelAccess labelAccess) {
    const wchar_t* szResult;
    switch (labelAccess) {
    case LabelAccess::UNKNOWN:
        szResult = L"UNKNOWN";
        break;

    case LabelAccess::READ:
        szResult = L"READ";
        break;

    case LabelAccess::WRITE:
        szResult = L"WRITE";
        break;

    case LabelAccess::READ_WRITE:
        szResult = L"READ_WRITE";
        break;

    case LabelAccess::IMMEDIATE:
        szResult = L"IMMEDITE";
        break;

    default:
        szResult = L"UNKOWN";
        break;
    }
    return szResult;
}

void AssertAccessEquals(LabelAccess actualLabelAccess, LabelAccess expectedLabelAccess) {
    if (actualLabelAccess != expectedLabelAccess) {
        Assert::Fail(wstringstream() << L"Actual access " << GetAccessText(actualLabelAccess) << L" is different from expected access " << GetAccessText(expectedLabelAccess));
    }
}

void AssertEquateEquals(wstring_view actualLine, EquateType expectedEquateType, wstring_view expectedLabel, LabelAccess expectedLabelAccess, Memory::address expectedAddress, wstring_view expectedComment, wstring_view expectedError) {
    EquateType actualEquateType;
    wstring actualLabel;
    LabelAccess actualLabelAccess;
    Memory::address actualAddress;
    wstring actualComment;
    wstringstream actualError;

    Equate::ReadFrom(actualLine, actualEquateType, actualLabel, actualLabelAccess, actualAddress, actualComment, actualError);

    try {
        AssertEquateTypeEquals(actualEquateType, expectedEquateType);
        Assert::StringEquals(actualLabel, expectedLabel);
        AssertAccessEquals(actualLabelAccess, expectedLabelAccess);
        Assert::LongEquals(actualAddress, expectedAddress);
        Assert::StringEquals(actualComment, expectedComment);
        Assert::StringEquals(actualError.str(), expectedError);

    }
    catch (const AssertionError& ex) {
        Assert::Fail(wstringstream() << "Equate line '" << actualLine << "' was not read as expected.");
    }

}

void EquateTest::TestEquate() {
    TestParsing();
}

void EquateTest::TestParsing() {

    // Scucces cases.
    AssertEquateEquals(L"", EquateType::EMPTY, L"", LabelAccess::UNKNOWN, 0, L"", L"");
    AssertEquateEquals(L" ;Comment", EquateType::COMMENT, L"", LabelAccess::UNKNOWN, 0, L"Comment", L"");
    AssertEquateEquals(L"DECIMAL = 1234  ; Comment", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 1234, L"Comment", L"");
    AssertEquateEquals(L"HEX = $1234 ; Comment", EquateType::LABEL, L"HEX", LabelAccess::READ_WRITE, 0x1234, L"Comment", L"");
    AssertEquateEquals(L"HEX+1 = $1234 ; Comment", EquateType::LABEL, L"HEX+1", LabelAccess::READ_WRITE, 0x1234, L"Comment", L"");
    AssertEquateEquals(L"HEX-12 = $1234 ; Comment", EquateType::LABEL, L"HEX-12", LabelAccess::READ_WRITE, 0x1234, L"Comment", L"");


    // Trailing comment.
    AssertEquateEquals(L"DECIMAL = 1234", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 1234, L"", L"");
    AssertEquateEquals(L"DECIMAL = 1234;", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 1234, L"", L"");
    AssertEquateEquals(L"DECIMAL = 1234 ;", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 1234, L"", L"");

    // Access qualifier.
    AssertEquateEquals(L"DECIMAL < 1234  ; Comment", EquateType::LABEL, L"DECIMAL", LabelAccess::READ, 1234, L"Comment", L"");
    AssertEquateEquals(L"DECIMAL > 1234  ; Comment", EquateType::LABEL, L"DECIMAL", LabelAccess::WRITE, 1234, L"Comment", L"");
    AssertEquateEquals(L"DECIMAL # 1234  ; Comment", EquateType::LABEL, L"DECIMAL", LabelAccess::IMMEDIATE, 1234, L"Comment", L"");

    // Error cases.
    AssertEquateEquals(L" = 123", EquateType::UNKNOWN, L"", LabelAccess::UNKNOWN, 0, L"", L"Character '=' at position 1 is not a valid start character for a label name.");

    AssertEquateEquals(L"DECIMAL", EquateType::LABEL, L"DECIMAL", LabelAccess::UNKNOWN, 0, L"", L"No access qualifier specified.");
    AssertEquateEquals(L"DECIMAL * ", EquateType::LABEL, L"DECIMAL", LabelAccess::UNKNOWN, 0, L"", L"Character '*' at position 9 is not an access qualifier. Use '=', '<', '>' or '#'.");
    AssertEquateEquals(L"DECIMAL = ", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 0, L"", L"No value specified.");
    AssertEquateEquals(L"DECIMAL = ; Comment", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 0, L"", L"Characters '; Comment' at position 11 cannot be interpreted as a decimal number.");
    AssertEquateEquals(L"DECIMAL = abc", EquateType::LABEL, L"DECIMAL", LabelAccess::READ_WRITE, 0, L"", L"Characters 'abc' at position 11 cannot be interpreted as a decimal number.");
    AssertEquateEquals(L"HEX = $; Comment", EquateType::LABEL, L"HEX", LabelAccess::READ_WRITE, 0, L"", L"Characters '; Comment' at position 8 cannot be interpreted as a hexadecimal number.");
    AssertEquateEquals(L"HEX = $xyz", EquateType::LABEL, L"HEX", LabelAccess::READ_WRITE, 0, L"", L"Characters 'xyz' at position 8 cannot be interpreted as a hexadecimal number.");

}


void EquateTest::TestFiles(wstring_view filePath) {
    //Assert::Fail(filePath);
}

