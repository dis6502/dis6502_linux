#pragma once

#include "InputStream.h"
#include <iostream>
#include <fstream>
#include "FileIO.h"

class FileInputStream : public InputStream {

public:
    FileInputStream() = default;
    virtual ~FileInputStream();

    FileIO::FILE_SIZE OpenFile(wstring_view filePath, const std::ios_base::openmode flags = std::ios_base::in | std::ios_base::binary);

    //bool IsEOF() const override;
    void Read(void* buffer, stream_size size) override;  // throws IOException
    void Skip(stream_offset offset) override;  // throws IOException;
    stream_position GetPosition() override;
private:
    wstring filePath;
    std::ifstream fs;
};
