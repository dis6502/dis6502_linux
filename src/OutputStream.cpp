#include "OutputStream.h"

#include "PlatformCompat.h"
#include "Encoding.h"
#include "File.h"
#include "FileIO.h"
#include "Strings.h"
#include "Text.h"
#include <filesystem>
#include <io.h>
#include <sys/stat.h>
#include <vector>

gsl::not_null<OutputStream*> OutputStream::OpenFile(wstring_view filePath, Encoding encoding) {
    if (encoding == Encoding::UNKNOWN) {
        throw IOException(L"Cannot write files if encoding is unknown");
    }
    auto folderPath = File(filePath).GetParentPath();
    if (!FileIO::FileExists(folderPath)) {
        std::filesystem::create_directories(std::filesystem::path(folderPath));
    }
    int fd = _wsopen(wstring(filePath).c_str(), _O_CREAT | _O_TRUNC | _O_WRONLY | EncodingFactory::GetInfo(encoding).flags, _SH_DENYNO, _S_IWRITE); // ##c_str() OK
    if (fd < 0) {
        const auto error = errno;
        // Cannot open file "{0}" for write access. Error code {1} - "{2}".
        throw IOException(Text::Format(IDS_FILE_IO_EX_OPENING_FILE_FOR_WRITE_ACCESS, filePath, std::to_wstring(error), wstring(_wcserror(error))));
    }
    return new OutputStream(filePath, fd, encoding);
}

OutputStream::OutputStream(wstring_view filePath, int fd, Encoding encoding) {

    if (fd < 0) {
        throw std::runtime_error("Invalid file descriptor");
    }
    this->filePath = filePath;
    this->fd = fd;
    this->encoding = encoding;
}

OutputStream::~OutputStream() {
    _close(fd);
}

void OutputStream::WriteString(wstring_view stringView) {
    switch (encoding) {

    case Encoding::UNKNOWN: {
        throw std::runtime_error("Invalid encoding");
    }

    case Encoding::BINARY: {
        throw IOException(L"Cannot write strings if encoding is binary");
    }
    case Encoding::ASCII: {
        std::vector<char> buffer;
        buffer.resize(stringView.size());
        for (size_t i = 0; i < stringView.size(); i++) {
            wchar_t c = stringView.at(i);
            if ((c == 10) || (32 <= c && c <= 127)) {
                buffer[i] = c;
            }
            else {
                throw IOException(wstringstream() << "Character '" << c << "' (" << (unsigned int)c << ") at position " << i << " of string '" << stringView << "' is no ASCII character and cannot be written in ASCII encoding mode.");
            }
        }
        Write(buffer.data(), stringView.length());
        break;
    }
    case Encoding::ATASCII: {
        std::vector<char> buffer;
        buffer.resize(stringView.size());
        for (size_t i = 0; i < stringView.size(); i++) {
            wchar_t c = stringView.at(i);
            if (c <= 2555) {
                buffer[i] = c;
            }
            else {
                throw IOException(wstringstream() << "Character '" << c << "' (" << (unsigned int)c << ") at position " << i << " of string '" << stringView << "' is no ASCII character and cannot be written in ATASCII encoding mode.");
            }
        }
        Write(buffer.data(), stringView.length());
        break;
    }
    case Encoding::UTF8: {
        Write(stringView.data(), stringView.length() * sizeof(wchar_t));
        break;
    }

    } // End switch
}

void OutputStream::WriteString(const wchar_t* string) {
    auto size = String::Length(string);
    auto stringView = wstring_view(string, size);
    WriteString(stringView);
}

void OutputStream::Write(const void* buffer, long size) {
    const long effectiveSize = _write(fd, buffer, size);

    if (effectiveSize != size) {
        throw IOException(String::Format(L"Could only write {0} of {1} requested bytes", std::to_wstring(effectiveSize), std::to_wstring(size)));
    }
}
