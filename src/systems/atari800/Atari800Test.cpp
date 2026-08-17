#include "Atari800Test.h"

#include <sstream>
#include "../../Assert.h"
#include "../../InputStream.h"
#include "../../OutputStream.h"

#include "../../SegmentList.h"
#include "../../SegmentListInserter.h"

#include "../ComputerSystem.h"
#include "../ComputerSystemFactory.h"
#include "../../InputStream.h"
#include "../../FileInputStream.h"
#include "../../XML.h"
#include "../../FileIOTest.h"
#include "DiskImageFileInputStream.h"


void Atari800Test::AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view filePath, wstring_view outFilePath) {
    FileInputStream inputStream;
    const auto fileSize = inputStream.OpenFile(filePath);
    AssertSegmentListEquals(computerSystem, fileType, filePath, inputStream, fileSize, false, outFilePath);
}

void Atari800Test::AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view diskImageFilePath, wstring_view fileName, bool expectReadFailure, wstring_view outFilePath) {
    DiskImageFileInputStream inputStream;
    FileIO::FILE_SIZE fileSize = 0;
    try {
        fileSize = inputStream.OpenDiskImageFile(diskImageFilePath, fileName);
    }
    catch (const IOException& ex) {
        if (expectReadFailure) {
            Assert::Log(wstringstream() << L"Expected failure '" << ex.GetText() << L"' when reading '" << fileName << "' on disk image '" << diskImageFilePath << "' detected correctly.");
            return;
        }
        else {
            Assert::Log(wstringstream() << L"I/O error '" << ex.GetText() << L"' when reading '" << fileName << L"' on disk image '" << diskImageFilePath << "'.");
            Assert::Fail(ex);
        }
    }
    auto filePath = (wstringstream() << diskImageFilePath << ".files" << FileIO::FILE_SEPARATOR << fileName).str();
    AssertSegmentListEquals(computerSystem, fileType, filePath, inputStream, fileSize, expectReadFailure, outFilePath);
}

void Atari800Test::AssertSegmentListEquals(const ComputerSystem& computerSystem, FileType fileType, wstring_view filePath, InputStream& inputStream,
    FileIO::FILE_SIZE fileSize, bool expectReadFailure, wstring_view outFilePath) {

    // Read the source file
    Assert::Log((wstringstream() << "Reading " << filePath << " as type " << FileTypeFactory::GetInfo(fileType).key).str());
    SegmentList actualSegmentList(nullptr); // SegmentList without equate handling
    auto segmentListInserter = actualSegmentList.CreateInserter();
    try {
        computerSystem.ReadFile(fileType, inputStream, fileSize, *segmentListInserter);
        segmentListInserter->Apply();
    }
    catch (const IOException& ex) {
        segmentListInserter->Cancel();

        // If this is a test for corrupted files, return with OK.
        if (expectReadFailure) {
            Assert::Log(wstringstream() << L"Expected failure '" << ex.GetText() << L"' detected correctly.");
            return;
        }
        else {
            Assert::Log(ex);
        }
        Assert::Fail(L"Computer system cannot parse input stream");
    }
    Assert::Log((wstringstream() << actualSegmentList.GetCount() << " segments detected.").str());

    // Save the segments list as XML
    const auto xmlOutFilePath = wstring(outFilePath) + L".xml";
    auto error = XML::Save(actualSegmentList, L"Segments", xmlOutFilePath);
    Assert::LongEquals(error, XML::Error::XML_SUCCESS);

    // Load the expected segments list as XML
    SegmentList expectedSegmentList(nullptr); // SegmentList without equate handling
    const auto xmlRefFilePath = wstring(filePath) + L".xml";
    error = XML::Load(expectedSegmentList, L"Segments", xmlRefFilePath);
    Assert::LongEquals(error, XML::Error::XML_SUCCESS);

    // Assert the XML files are equal
    FileIOTest::AssertFileEquals(xmlOutFilePath, xmlRefFilePath);

    // Export the segments as binary file, too.
    const auto exportedFilePath = wstring(outFilePath) + L".exported";
    const auto outputStream = OutputStream::OpenFile(exportedFilePath, Encoding::BINARY);
    computerSystem.WriteExecutableFile(actualSegmentList, NO_SEGMENT_INDEX, true, *outputStream);
    delete outputStream;

    // Assert the binary files are equal
    const auto exportedRefFilePath = wstring(filePath) + L".exported";

    FileIOTest::AssertFileEquals(exportedFilePath, exportedRefFilePath);

}
