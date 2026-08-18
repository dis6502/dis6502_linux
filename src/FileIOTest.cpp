#include "Assertions.h"
#include "FileIO.h"
#include "FileIOTest.h"
#include "Strings.h"


void FileIOTest::AssertFileEquals(wstring_view actualFilePath, wstring_view expectedFilePath) {
    Assert::Log(wstringstream() << "Comparing " << actualFilePath << " with " << expectedFilePath);
    if (!FileIO::FileExists(actualFilePath)) {
        Assert::Fail(wstringstream() << "Actual file " << actualFilePath << " does not exist");
    }
    if (!FileIO::FileExists(expectedFilePath)) {
        Assert::Fail(wstringstream() << "Expected file " << expectedFilePath << " does not exist");
    }
    const auto actualFileContent = FileIO::ReadByteArray(actualFilePath);
    const auto expectedFileContent = FileIO::ReadByteArray(expectedFilePath);

    Assert::LongEquals(actualFileContent.size(), expectedFileContent.size());
    for (size_t i = 0; i < actualFileContent.size(); i++) {
        const auto actualByte = actualFileContent.at(i);
        const auto expectedByte = expectedFileContent.at(i);
        if (actualByte != expectedByte) {
            Assert::Fail(wstringstream() << "Files differ at position " << Assert::LongToString(i) << ": actual = " << Assert::ByteToString(actualByte) << " expected = " << Assert::ByteToString(expectedByte));
        }
    }
}

void FileIOTest::TestFileIO() {

    auto filePath = String::ansi_to_wstring(".non_existing_test_file");
    try {
        const auto fileSize = FileIO::GetFileSize(filePath);
        Assert::Fail(L"Expected IOException instead of file size "+std::to_wstring(fileSize));
    }
    catch (const IOException&) {
        // OK
    }

    std::ofstream fout;
    fout.open(FileIO::ToPath(filePath), std::ios::binary | std::ios::out);

    constexpr size_t arraySize = 40000;
    auto byteArray = std::make_unique<char[]>(arraySize);
    fout.write(byteArray.get(), arraySize);
    fout.close();
    byteArray.reset();

    const auto fileSize = FileIO::GetFileSize(filePath);
    if (fileSize != arraySize) {
        throw  std::runtime_error("Length of existing file must be 40000");
    }
}
