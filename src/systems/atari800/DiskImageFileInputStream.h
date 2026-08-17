#pragma once

#include "../../Syntax.h"

#include "../../FileInputStream.h"

#include "AtariDOS.h"


class DiskImageFileInputStream final : public FileInputStream {
public:
    DiskImageFileInputStream();

    FileIO::FILE_SIZE OpenDiskImageFile(wstring_view diskImageFilePath, wstring_view fileName); // Throws IOException
    FileIO::FILE_SIZE GetFileSize() const;

    void Read(void* buffer, stream_size size) override; // throws IOException
    void Skip(stream_offset offset) override;  // throws IOException
    Stream::stream_position GetPosition() override;


private:
    wstring fileName;

    ByteArray fileContent;
    bool valid;
    long long position;

    void Reset();
};
