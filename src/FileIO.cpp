#include "FileIO.h"
#include "Strings.h"
#include "Text.h"
#include <filesystem>
#include <gsl/gsl>
#include "PlatformCompat.h"

#ifdef _WIN32
const wstring FileIO::FILE_SEPARATOR = L"\\";
#else
const wstring FileIO::FILE_SEPARATOR = L"/";
#endif
const wstring FileIO::EMPTY_FILE_PATH = L"";

bool FileIO::FileExists(wstring_view filePath) {
    return std::filesystem::exists(filePath);
}

bool FileIO::IsFileReadOnly(wstring_view filePath) {
    auto perms = std::filesystem::status(filePath).permissions();
    return ((perms & std::filesystem::perms::owner_write) == std::filesystem::perms::none);
}

FileIO::FILE_SIZE FileIO::GetFileSize(wstring_view filePath) {
    try {
        return std::filesystem::file_size(filePath);

    }
    catch (const std::filesystem::filesystem_error& ex) {
        throw IOException(String::Format(L"Cannot determine file size of file '{0}': {1}", filePath, String::utf8_to_wstring(ex.what())));
    }

}


ByteArray FileIO::ReadByteArray(wstring_view filePath) {

    std::ifstream fs(ToPath(filePath), std::ios::in | std::ios::binary);
    if (!fs.good() || !fs.is_open()) {
        throw IOException(String::Format(L"Cannot open file '{0}' for reading", filePath));
    }

    auto fileSize = std::filesystem::file_size(filePath);
#undef max
    if (fileSize > std::numeric_limits<std::size_t>::max()) {
        throw IOException(String::Format(L"Requested buffer size {0} exceeds the memory limits", std::to_wstring(fileSize)));
    }
    const auto bufferSize = gsl::narrow_cast<size_t>(fileSize);
    auto buffer = ByteArray(bufferSize);
    fs.read(reinterpret_cast<char*>(buffer.get()), buffer.size());
    if (fs.gcount() != fileSize) {
        throw IOException(String::Format(L"Cannot read expected {1} bytes from file \"{0}\". Only {2} bytes read.",
            filePath, std::to_hex_string(fileSize), std::to_hex_string(fs.gcount())));
    }
    return buffer;
}


wstring FileIO::ReadString(wstring_view filePath) {

    auto textBytes = FileIO::ReadByteArray(filePath);
    auto text = textBytes.to_wstring();
    return text;
}

std::vector<wstring> FileIO::ReadStrings(wstring_view filePath) {
    std::vector<wstring> result;
    auto fd = OpenFile(filePath, L"rt");

    auto buffer = String::szBuffer;
    while (fgetws(buffer, String::BUFFER_SIZE, fd) != nullptr) {

        // Terminate string with zero instead of newline, in case the newline is the last character.
        buffer[wcscspn(buffer, L"\r\n")] = 0;
        result.push_back(buffer);
    }
    CloseFile(fd);
    return result;
}

FILE* FileIO::OpenFile(wstring_view filePath, wstring_view mode) {
    FILE* fd = _wfopen(wstring(filePath).c_str(), wstring(mode).c_str());

    if (fd == 0) {
        errno_t errorNumber;

        _get_errno(&errorNumber);
        auto errorMessage = wstring(_wcserror(errorNumber));

        if (mode.starts_with(L"r")) {
            throw IOException(Text::Format(IDS_FILE_IO_EX_OPENING_FILE_FOR_READ_ACCESS, filePath, std::to_wstring(errorNumber), errorMessage));
        }
        else if (mode.starts_with(L"r+")) {
            throw IOException(Text::Format(IDS_FILE_IO_EX_OPENING_FILE_FOR_READ_WRITE_ACCESS, filePath, std::to_wstring(errorNumber), errorMessage));
        }
        else {
            throw IOException(Text::Format(IDS_FILE_IO_EX_OPENING_FILE_FOR_WRITE_ACCESS, filePath, std::to_wstring(errorNumber), errorMessage));
        }
    }
    return fd;
}

void FileIO::CloseFile(FILE* fd) {
    fclose(fd);
}

void FileIO::SetCurrentWorkingDirectory(wstring_view folderPath) {
#ifdef WIN32
    if (::SetCurrentDirectory(wstring(folderPath).c_str()) == 0) {
        throw IOException(String::Format(L"Cannot set folder path '{0}' as current directory", folderPath));
    }
#else 
    if (chdir(wstring(folderPath).c_str()) != 0) {
        throw IOException(String::Format(L"Cannot set folder path '{0}' as current directory", folderPath));
    }
#endif
}