#include "Application.h"
#include "ComputerSystem.h"
#include "ComputerSystemType.h"
#include "FileIO.h"
#include "FileType.h"
#include "InputStream.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include <algorithm>
#include <memory>
#include <Memory.h>
#include <OutputStream.h>
#include <SegmentTypes.h>
#include <sstream>
#include <stdexcept>
#include <Syntax.h>

extern std::unique_ptr<Application> g_Application;


ComputerSystem::ComputerSystem(const ComputerSystemTypeInfo& computerSystemTypeInfo) :
    computerSystemTypeInfo(&computerSystemTypeInfo) {}

ComputerSystem::~ComputerSystem() {}


ComputerSystemType ComputerSystem::GetType() const {
    return computerSystemTypeInfo->type;
}

gsl::not_null<const ComputerSystemTypeInfo*> ComputerSystem::GetTypeInfo() const {
    return computerSystemTypeInfo;
}

wstring ComputerSystem::GetResourceFilePath(wstring_view fileName) const {
    auto filePath = ::g_Application->GetModuleFilePath(L"systems");
    std::wostringstream ss;
    ss << filePath << FileIO::FILE_SEPARATOR << computerSystemTypeInfo->fileName << FileIO::FILE_SEPARATOR << fileName;
    return ss.str();
}

wstring ComputerSystem::GetResourceFilePathByExtension(wstring_view extension) const {
    std::wostringstream ss;
    ss << computerSystemTypeInfo->fileName << extension;
    return GetResourceFilePath(ss.str());
}

byte ComputerSystem::GetReturnCharacter() const {
    return returnCharacter;
}

bool ComputerSystem::IsSupportedFileType(FileType fileType) const {
    return std::find(supportedFileTypes.begin(), supportedFileTypes.end(), fileType) != supportedFileTypes.end();
}

bool ComputerSystem::IsDisplayListVectorAddress(const Memory::address address) const {
    return false;
}

FileType ComputerSystem::GuessFileType(wstring_view filePath) const {
    constexpr size_t headerSize = 4;

    auto byteArray = FileIO::ReadByteArray(filePath);
    const FileIO::FILE_SIZE fileSize = byteArray.size();

    if (fileSize < headerSize) {
        return FileType::UNKNOWN_FILE;
    }

    return GuessFileType(fileSize, *byteArray.GetSubSequence(0, headerSize));
}

void ComputerSystem::ReadFile(FileType fileType, InputStream& inputStream, FileIO::FILE_SIZE fileSize, SegmentListInserter& segmentListInserter) const {
    if (!IsSupportedFileType(fileType)) {
        throw std::logic_error("File type is not supported.");
    }

    if (fileType == FileType::CASSETTE_IMAGE_FILE) {
        ReadCassetteFile(segmentListInserter, inputStream, fileSize);
    }
    else  if (fileType == FileType::EXECUTABLE_FILE) {
        ReadExecutableFile(segmentListInserter, inputStream, fileSize);
    }
    else if (fileType == FileType::ROM_IMAGE_FILE) {
        ReadROMFile(segmentListInserter, inputStream, fileSize);
    }
    else if (fileType == FileType::DISK_IMAGE_EXECUTABLE_FILE) {
        throw std::logic_error("File type is not supported.");
    }
    else if (fileType == FileType::DISK_IMAGE_BOOT_SECTORS) {
        throw std::logic_error("File type is not supported.");
    }
    else if (fileType == FileType::DISK_IMAGE_SECTORS) {
        throw std::logic_error("File type is not supported.");
    }
    else {
        throw std::logic_error("File type is not supported.");
    }

}

void ComputerSystem::ReadCassetteFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    throw std::logic_error("Operation is not supported.");
}

void ComputerSystem::ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    throw std::logic_error("Operation is not supported.");
}


// TODO: Have first & lastSegmentIndex?
void ComputerSystem::WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool writeHeader, OutputStream& lpOutputStream) const {
    throw std::logic_error("Operation is not supported.");
}

void ComputerSystem::ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const {
    throw std::logic_error("Operation is not supported.");
}
