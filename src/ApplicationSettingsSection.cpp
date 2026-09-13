#include <cstddef>
#define NOMINMAX

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "Syntax.h"
#ifdef _WIN32
#include <Windows.h> // For GetPrivateProfileString/WritePrivateProfileString
#endif

ApplicationSettingsSection::ApplicationSettingsSection(Application& application, wstring_view settingsFilePath, wstring_view name) :
    application(&application), settingsFilePath(settingsFilePath), name(name) {}

void ApplicationSettingsSection::GetString(wstring_view keyName, wstring& value, wstring_view defaultValue) const {
#ifdef  _WIN32
    constexpr size_t MAX_LENGTH = 256;
    wchar_t buffer[MAX_LENGTH] = { };
    LPWSTR lpBuffer = buffer;
    GetPrivateProfileString(wstring(name).c_str(), wstring(keyName).c_str(), wstring(defaultValue).c_str(), lpBuffer, MAX_LENGTH - 1, wstring(settingsFilePath).c_str()); // #.c_str() OK
    value = wstring(lpBuffer);
#else
    value = defaultValue;
#endif

}

void ApplicationSettingsSection::WriteString(wstring_view keyName, wstring_view value) const {
#ifdef  _WIN32
    WritePrivateProfileString(wstring(name).c_str(), wstring(keyName).c_str(), wstring(value).c_str(), wstring(settingsFilePath).c_str()); // #.c_str() OK
#endif
}

void ApplicationSettingsSection::GetUnsignedInt(wstring_view keyName, unsigned int& value, unsigned int defaultValue) const {
#ifdef  _WIN32
    value = GetPrivateProfileInt(wstring(name).c_str(), wstring(keyName).c_str(), defaultValue, wstring(settingsFilePath).c_str()); // #.c_str() OK
#else
    value = defaultValue;
#endif
}
