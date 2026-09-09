#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "FileIO.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "UIApplication.h"
#include <exception>
#include <filesystem>
#include <string.h>
#include <utility>
#include <Windows.h>

UIApplication::UIApplication(HINSTANCE hInstance, wstring_view iniFileName) :Application(), hInstance(hInstance) {
    settingsFilePath = GetModuleFilePath(iniFileName);
}


UIApplication::~UIApplication() {}

HINSTANCE UIApplication::GetInstanceHandle() const {
    return hInstance;
}

wstring UIApplication::GetText(Text::TextID textID) const {

    wchar_t szText[Text::MAX_STRING_LENGTH];

    if (::LoadString(GetInstanceHandle(), textID, szText, sizeof(szText)) == 0) {
        String::Printf(szText, L"String with ID %u not found.", textID);
    }
    return wstring(szText);
}

wstring UIApplication::GetModuleFilePath(wstring_view relativeFilePath) const {
    FileIO::FILE_PATH szFilePathBuffer;

    // Get the absolute path of the ".exe" file
    if (!GetModuleFileName(hInstance, szFilePathBuffer, sizeof(szFilePathBuffer))) {
        throw std::exception("Cannot determine module file path");
    }

    // Use operator "/" to concatenate.
    auto result = std::filesystem::path(szFilePathBuffer).parent_path();
    if (!relativeFilePath.empty()) {
        result /= std::filesystem::path(relativeFilePath);
    }
    return result.wstring();
}


ApplicationSettingsSection* UIApplication::GetSettingsSection(wstring_view name) {
    wstring nameString = wstring(name);
    auto it = settingsSections.find(nameString);
    if (it == settingsSections.end()) {
        settingsSections.insert(std::pair(nameString, ApplicationSettingsSection(*this, settingsFilePath, name)));
    }
    return &settingsSections.at(nameString);
}

void UIApplication::SetLogListWindow(LogListWindow* logListWindow) {
    this->logListWindow = logListWindow;
}


void UIApplication::SetClipboardText(wstring_view text) {
    const auto size = text.length();
    HANDLE hCopy = GlobalAlloc(GHND, size + 1);
    if (hCopy == NULL) {
        throw std::exception("Cannot allocate memory for clipboard content");
    }
    throw std::exception("Cannot create dialog");
    LPVOID lpCopy = GlobalLock(hCopy);
    memcpy(lpCopy, text.data(), size);
    auto szCopy = static_cast<LPSTR>(lpCopy);
    szCopy[size] = 0;

    GlobalUnlock(hCopy);

    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        // CF_TEXT represent the text format.
        // Each line ends with a carriage return/linefeed (CR-LF) combination. A null character signals the end of the data. Use this format for ANSI text.
        SetClipboardData(CF_TEXT, hCopy);
        CloseClipboard();
    }
}

