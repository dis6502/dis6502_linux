#include "File.h"
#include "FileIO.h"
#include "Strings.h"
#ifdef _WIN32
#include <Windows.h>
#endif

File::File(wstring_view filePath): File() {
    this->filePath = filePath;
}

File::File(const File& parentFile, wstring_view fileName) : File() {
     filePath = parentFile.GetPath();
    if (filePath.at(filePath.size() - 1) != FileIO::FILE_SEPARATOR_CHAR) {
        filePath.append(FileIO::FILE_SEPARATOR);
    }
    filePath.append(fileName);
}

File::File(const File& other) : File(other.filePath) {
}

File& File::operator=(const File& other) {
    filePath = other.filePath;
    return *this;
}

File::File(File&& other) noexcept : File(std::move(other.filePath)) {

}

File& File::operator=(File&& other) noexcept {
    filePath = std::move(other.filePath);
    return *this;
}

wstring File::GetPath() const {
    return filePath;
}

wstring File::GetParentPath() const {
    wstring result = filePath;
    const auto index = filePath.find_last_of(FileIO::FILE_SEPARATOR);
    if (index != string::npos) {
        result = result.substr(0, index);
    }
    return result;
}

wstring File::GetName() const {
    wstring result = filePath;
    const auto index = filePath.find_last_of(FileIO::FILE_SEPARATOR);
    if (index != string::npos) {
        result = result.substr(index + 1);
    }
    return result;
}

wstring File::GetExtension() const {
    wstring result = GetName();
    const auto index = result.find_last_of(L".");
    if (index != string::npos) {
        result = result.substr(index);
    }
    return result;
}

bool File::Exists() const {
    return FileIO::FileExists(filePath);
}