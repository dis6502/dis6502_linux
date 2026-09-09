#pragma once

#include "ApplicationSettingsSection.h"
#include "Syntax.h"
#include "Text.h"
#include <exception>
#include <map>

typedef char TITLE[80];

class Window;

class Application {

public:
    virtual ~Application();

    virtual wstring GetModuleFilePath(wstring_view relativeFilePath) const = 0;;

    virtual ApplicationSettingsSection* GetSettingsSection(wstring_view name) = 0;

    // Message handling
    void SendInfoMessageWithID(Text::TextID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");

    void SendErrorMessageWithID(Text::TextID textID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");
    void ThrowErrorMessageWithID(Text::TextID textID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"") const;
    void SendErrorMessageWithException(const std::exception& ex);

protected:
    std::map<wstring, ApplicationSettingsSection> settingsSections;

    Application();


    virtual void SendLogMessage(const wchar_t* szText);
};
