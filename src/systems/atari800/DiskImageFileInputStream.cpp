#include "AtariDOS.h"

#include "DiskImageFileInputStream.h"

DiskImageFileInputStream::DiskImageFileInputStream() : valid(false), position(0) {
}


FileIO::FILE_SIZE DiskImageFileInputStream::OpenDiskImageFile(wstring_view diskImageFilePath, wstring_view fileName) {
    auto atariDisk = AtariDOS::OpenAtariDisk(diskImageFilePath);
    this->fileName = fileName;

    Reset();

    fileContent = atariDisk->ReadFile(fileName);
    valid = true;
    return fileContent.size();
}

void DiskImageFileInputStream::Reset() {
    fileContent = ByteArray();
    valid = false;
    position = 0;
}


FileIO::FILE_SIZE DiskImageFileInputStream::GetFileSize() const {
    return fileContent.size();
}


void DiskImageFileInputStream::Read(void* buffer, stream_size size) {
    if (buffer == nullptr) {
        throw std::runtime_error("Buffer must not be nullptr");
    }

    if (size > (stream_size)std::numeric_limits<size_t>::max()) {
        throw IOException(L"Specified file size exceeds the memory limit");
    }

    const auto endPosition = position + size;
    if (size > 0) {
        if (endPosition <= (stream_size)GetFileSize()) {
            memcpy(buffer, fileContent.get() + position, (size_t)size);
        }
    }
    Skip(size);
};

void DiskImageFileInputStream::Skip(stream_offset offset) {

    if (!valid) {
        throw IOException(L"End of file already reached");
    }
    position = position + offset;
    if (position < 0) {
        throw IOException(L"Invalid negative position");

    }
    const auto result = (position <= (stream_position)GetFileSize());
    if (!result) {
        valid = false;
    }

    if (!result) {
        throw IOException(L"Specified offset exceeds end of file");

    }
};

Stream::stream_position DiskImageFileInputStream::GetPosition() {
    return position;
}

