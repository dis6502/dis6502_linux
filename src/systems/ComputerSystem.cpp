#include <sstream>

#include "Application.h"
#include "FileIO.h"
#include "FileType.h"
#include "InputStream.h"
#include "MessageBoxDialog.h"
#include "Segment.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include "Strings.h"


#include "ComputerFont.h"
#include "ComputerSystem.h"
#include "ComputerSystemFactory.h"

extern std::unique_ptr<Application> g_Application;


ComputerSystem::ComputerSystem(const ComputerSystemTypeInfo& computerSystemTypeInfo):
   computerSystemTypeInfo(&computerSystemTypeInfo) {

    computerFont = std::make_unique<ComputerFont>(8);
}

ComputerSystem::~ComputerSystem(){
   RemoveFontResource(fontFilePath.c_str());
}


ComputerSystemType ComputerSystem::GetType() const  {
    return computerSystemTypeInfo->type;
}

gsl::not_null<const ComputerSystemTypeInfo*> ComputerSystem::GetTypeInfo() const  {
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

HFONT ComputerSystem::GetFont(bool doubleHeight) const  {
    return computerFont->GetFont(doubleHeight);
}

byte ComputerSystem::GetReturnCharacter() const  {
    return returnCharacter;
}

bool ComputerSystem::IsSupportedFileType(FileType fileType) const {
    return std::find(supportedFileTypes.begin(), supportedFileTypes.end(), fileType) != supportedFileTypes.end();
}

void ComputerSystem::Init() {

    // Load font.
    //fontFilePath = GetResourceFilePath("AtariClassic-Regular.ttf");
    //string fontName = "Atari Classic";
    //if (FileIO::FileExists(fontFilePath)) {

    // TODO: Currently TTF is not yet supported as it would required Unicode. 
    // See https://sourceforge.net/p/dis6502/bugs/33/
    // TODO: This means we can try this now!
    fontFilePath = GetResourceFilePathByExtension(L".fon");
    wstring fontName = L"Atari800";
    //}
    if (AddFontResource(fontFilePath.c_str()) == 0) {
        auto message = String::Format(L"Font file '{0}' not found.", fontFilePath);
        MessageBoxDialog::Show(nullptr, GetTypeInfo()->id, message, MB_OK);
    }
    computerFont->Load(fontName);
}

bool ComputerSystem::IsDisplayListVectorAddress(const Memory::address address) const {
    return false;
}

FileType ComputerSystem::GuessFileType(wstring_view filePath) const {
    constexpr size_t headerSize = 4;
    byte buf[headerSize] = { 0,0,0,0 };

    auto byteArray = FileIO::ReadByteArray(filePath);
    const FileIO::FILE_SIZE fileSize = byteArray.size();

    if (fileSize < headerSize) {
        return FileType::UNKNOWN_FILE;
    }
    buf[0] = byteArray.at(0);
    buf[1] = byteArray.at(1);
    buf[2] = byteArray.at(2);
    buf[3] = byteArray.at(3);
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
