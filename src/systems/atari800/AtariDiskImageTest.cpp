#include "AtariDiskImageTest.h"

#include <sstream>
#include <gsl/util>

#include "AtariDiskImage.h"
#include "AtariDOS.h"
#include "DiskImageFileInputStream.h"
#include "Assertions.h"
#include "FileInputStream.h"
#include "StreamTest.h"

void AtariDiskImageTest::AssertFileNames83Equal() {
    AssertFileName83Equals(L"", L"");
    AssertFileName83Equals(L"DOS.SYS", L"DOS     .SYS");
    AssertFileName83Equals(L"AUTORUN.SYS", L"AUTORUN .SYS");
    AssertFileName83Equals(L"ABCDEFGH.ABC", L"ABCDEFGH.ABC");
}

void AtariDiskImageTest::AssertFileName83Equals(wstring_view fileName, wstring_view expectedFileName83) {
    Assert::Log(wstring(L"Checking file name ") + wstring(fileName));

    auto actualFileName83 = AtariDOS::GetFileName83(fileName);
    if (actualFileName83 != expectedFileName83) {
        Assert::LogValue(L"Actual File Name 8.3  ", actualFileName83);
        Assert::LogValue(L"Expected File Name 8.3", expectedFileName83);
        Assert::Fail(L"Different file names 8.3");

    }
}

void AtariDiskImageTest::AssertDirectoryEquals(wstring_view diskImageFilePath, wstring_view expectedDirectory) {

    Assert::Log(wstring(L"Checking directory of disk image ") + wstring(diskImageFilePath));


    auto actualDirectory = GetDirectory(diskImageFilePath);
    auto expectedDirectoryString = wstring(expectedDirectory);
    if (actualDirectory != expectedDirectoryString) {
        Assert::LogValue(L"Disk Image", diskImageFilePath);
        Assert::LogValue(L"Actual Directory", L"\n" + actualDirectory);
        Assert::LogValue(L"Expected Directory", L"\n" + expectedDirectoryString);
        Assert::Fail(L"Different directories");
    }

    // Also compare the actual contents retrieved from the disk image with the reference copies in the ".files" folder.
    const auto atariDisk = AtariDOS::OpenAtariDisk(diskImageFilePath);
    AtariFile info;
    auto error = atariDisk->FindFirst(info);
    while (error == AtariError::OK) {
        auto fileName = info.GetFileName();
        auto expectedFilePath = wstring(diskImageFilePath) + L".files" + FileIO::FILE_SEPARATOR + fileName;
        AssertFileContentEquals(diskImageFilePath, fileName, expectedFilePath);
        error = atariDisk->FindNext(info);
    }
}

wstring AtariDiskImageTest::GetDirectory(wstring_view diskImageFilePath) {
    wstringstream result;
    AtariFile info;

    const auto atariDisk = AtariDOS::OpenAtariDisk(diskImageFilePath);
    auto error = atariDisk->FindFirst(info);
    while (error == AtariError::OK) {
        result << info.GetDirectoryText() << std::endl;
        error = atariDisk->FindNext(info);
    }
    result << (wstring_view)AtariDOS::GetErrorCode(error) << std::endl;

    return result.str();
}


void AtariDiskImageTest::AssertFileContentEquals(wstring_view diskImageFilePath, wstring_view fileName, wstring_view expectedFilePath) {

    Assert::Log(wstring(L"Checking file ") + wstring(fileName));

    // Read using full content reads
    FileIO::FILE_SIZE actualReadCount;
    auto actualFileContent = GetFileContent(diskImageFilePath, fileName, actualReadCount);

    auto expectedFileContent = FileIO::ReadByteArray(expectedFilePath);

    if (actualReadCount != expectedFileContent.size()) {
        Assert::LogValue(L"Disk Image", diskImageFilePath);
        Assert::LogValue(L"File Name", fileName);
        Assert::LogValue(L"Expected File Path", expectedFilePath);

        Assert::LogValue(L"Actual File Content Size  ", std::to_wstring(actualReadCount));
        Assert::LogValue(L"Expected File Content Size", std::to_wstring(expectedFileContent.size()));
        Assert::Fail(L"Different file size");
    }
    auto size = gsl::narrow_cast<size_t>(actualReadCount);
    if (memcmp(actualFileContent.get(), expectedFileContent.get(), size) != 0) {
        Assert::LogValue(L"Disk Image", diskImageFilePath);
        Assert::LogValue(L"File Name", fileName);
        Assert::LogValue(L"Expected File Path", expectedFilePath);

        Assert::LogValue(L"Actual File Content   ", DatatypeUtility::ByteArrayToHexString(actualFileContent.get(), size, true));
        Assert::LogValue(L"Expected File Content ", expectedFileContent.to_wstring());
        Assert::Fail(L"Different file content");
    }

    // Read using input streams
    DiskImageFileInputStream actualFileInputStream;
    const auto actualFileInputStreamSize = actualFileInputStream.OpenDiskImageFile(diskImageFilePath, fileName);
    FileInputStream expectedFileInputStream;
    const auto expectedFileInputStreamSize = expectedFileInputStream.OpenFile(expectedFilePath);
    if (actualFileInputStreamSize != expectedFileInputStreamSize) {
        Assert::LogValue(L"Disk Image", diskImageFilePath);
        Assert::LogValue(L"File Name", fileName);
        Assert::LogValue(L"Expected File Path", expectedFilePath);

        Assert::LogValue(L"Actual File Input Stream Size  ", std::to_wstring(actualFileInputStreamSize));
        Assert::LogValue(L"Expected File Input Stream Size", std::to_wstring(expectedFileInputStreamSize));
        Assert::Fail(L"Different file input stream size");
    }

    StreamTest::AssertInputStreamsEqual(actualFileInputStream, expectedFileInputStream, expectedFileInputStreamSize);

}

ByteArray AtariDiskImageTest::GetFileContent(wstring_view diskImageFilePath, wstring_view fileName, FileIO::FILE_SIZE& readCount) {

    ByteArray result;
    AtariFile info;

    readCount = 0;
    const auto atariDisk = AtariDOS::OpenAtariDisk(diskImageFilePath);
    auto error = atariDisk->FindFirst(info);
    while (error == AtariError::OK) {
        if (info.GetFileName() == fileName) {
            AtariDOS::file_size fileSize;
            if (atariDisk->GetFileSize(info, fileSize) == AtariError::OK) {
                readCount = fileSize;
                result = ByteArray(fileSize);
                int iDiskIndex = -1;
                atariDisk->ReadFile(info, iDiskIndex, result.get(), fileSize);
            }
            break;
        }
        else {
            error = atariDisk->FindNext(info);
        }
    }

    return result;
}
