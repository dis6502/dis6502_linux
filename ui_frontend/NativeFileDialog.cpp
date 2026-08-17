#include "NativeFileDialog.h"

#include <nfd.h>

#include <cstdio>
#include <sstream>

#include "String.h"

namespace {

// Converts FileTypeFactory's ".bin;.com;.exe" style extension list into
// NFD's "bin,com,exe" style (no leading dots, comma-separated). A bare
// ".*" wildcard (used by RAW_FILE/UNKNOWN_FILE) becomes an empty spec,
// which the caller below treats as "no filter, show everything".
std::string ToNfdSpec(const wstring& filterExtensions) {
    std::string utf8 = String::wstring_to_utf8(filterExtensions);
    std::string result;
    std::stringstream ss(utf8);
    std::string token;
    bool first = true;
    while (std::getline(ss, token, ';')) {
        if (!token.empty() && token[0] == '.') token = token.substr(1);
        if (token == "*" || token.empty()) continue;
        if (!first) result += ",";
        result += token;
        first = false;
    }
    return result;
}

} // namespace

void NativeFileDialog::Init() {
    NFD_Init();
}

void NativeFileDialog::Shutdown() {
    NFD_Quit();
}

std::optional<std::string> NativeFileDialog::OpenFile(FileType fileType) {
    auto info = FileTypeFactory::GetInfo(fileType);
    std::string specUtf8 = ToNfdSpec(info.filterExtensions);
    std::string nameUtf8 = String::wstring_to_utf8(info.filterText);

    nfdfilteritem_t filterItem[1] = { { nameUtf8.c_str(), specUtf8.c_str() } };
    nfdchar_t* outPath = nullptr;

    nfdresult_t result = specUtf8.empty()
        ? NFD_OpenDialog(&outPath, nullptr, 0, nullptr)
        : NFD_OpenDialog(&outPath, filterItem, 1, nullptr);

    if (result == NFD_OKAY) {
        std::string path(outPath);
        NFD_FreePath(outPath);
        return path;
    }
    if (result == NFD_ERROR) {
        std::fprintf(stderr, "NFD_OpenDialog error: %s\n", NFD_GetError());
    }
    return std::nullopt; // NFD_CANCEL, or NFD_ERROR (already logged above)
}

std::optional<std::string> NativeFileDialog::SaveFile(FileType fileType, const std::string& defaultName) {
    auto info = FileTypeFactory::GetInfo(fileType);
    std::string specUtf8 = ToNfdSpec(info.filterExtensions);
    std::string nameUtf8 = String::wstring_to_utf8(info.filterText);

    nfdfilteritem_t filterItem[1] = { { nameUtf8.c_str(), specUtf8.c_str() } };
    nfdchar_t* outPath = nullptr;
    const nfdchar_t* defaultNamePtr = defaultName.empty() ? nullptr : defaultName.c_str();

    nfdresult_t result = specUtf8.empty()
        ? NFD_SaveDialog(&outPath, nullptr, 0, nullptr, defaultNamePtr)
        : NFD_SaveDialog(&outPath, filterItem, 1, nullptr, defaultNamePtr);

    if (result == NFD_OKAY) {
        std::string path(outPath);
        NFD_FreePath(outPath);
        return path;
    }
    if (result == NFD_ERROR) {
        std::fprintf(stderr, "NFD_SaveDialog error: %s\n", NFD_GetError());
    }
    return std::nullopt;
}
