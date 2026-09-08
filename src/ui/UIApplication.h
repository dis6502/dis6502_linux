#pragma once

#include "Application.h"
#include <Windows.h>

class LogListWindow;

class UIApplication : public Application {

public:

    UIApplication(HINSTANCE hInstance, wstring_view iniFileName);
    virtual ~UIApplication();

    HINSTANCE GetInstanceHandle() const;

    wstring GetModuleFilePath(wstring_view relativeFilePath) const;
    ApplicationSettingsSection* GetSettingsSection(wstring_view name);

    void SetLogListWindow(LogListWindow* logListWindow);

    void SetClipboardText(wstring_view text);


private:
    HINSTANCE hInstance;
    wstring settingsFilePath;

    LogListWindow* logListWindow = nullptr;
};


