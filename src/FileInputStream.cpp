#include "FileInputStream.h"
#include "PlatformCompat.h"


#include "Debug.h"
#include "Strings.h"
#include <sstream>

FileInputStream::~FileInputStream() {
    fs.close();
}

FileIO::FILE_SIZE FileInputStream::OpenFile(wstring_view filePath, const std::ios_base::openmode flags) {
    this->filePath = filePath;
    if (!FileIO::FileExists(filePath)) {
        throw IOException(String::Format(L"File '{0}' does not exist", filePath));
    }
    fs.open(ToPath(this->filePath), std::ios::in | flags);
    if (!fs.is_open()) {
        throw IOException(String::Format(L"Cannot open file '{0}' for reading", filePath));
    }
    if (!fs.good()) {
        throw IOException(String::Format(L"Cannot access file '{0}'", filePath));
    }
    return FileIO::GetFileSize(filePath);
}


// If this does not read all data, check if you have specified the std::ios::binary flags
void FileInputStream::Read(void* buffer, stream_size size) {

    if (size < 0) {
        throw std::runtime_error(String::wstring_to_utf8(String::Format(L"Stream size {0} must not be negative", std::to_wstring(size))));
    }
    if (fs.eof()) {
        throw IOException(L"The stream has already be read completey");
    }

    if (size > 0) {
        const auto currentOffset = fs.tellg();
        const auto effectiveSize = fs.read((char*)buffer, size).gcount();
        if (effectiveSize < size) {
            throw IOException(String::Format(L"Only {0} of the requested {1} bytes could be read at the current offset {2}",
                std::to_wstring(effectiveSize), std::to_wstring(size), std::to_wstring(currentOffset)));
        }
    }
}

void FileInputStream::Skip(stream_offset offset) {
    fs.seekg(offset, fs.cur);

    if (fs.fail()) {
        throw IOException(L"Specified offset exceeds end of file");
    };

}
Stream::stream_position FileInputStream::GetPosition() {
    return fs.tellg();
}

