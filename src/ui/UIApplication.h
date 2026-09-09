#pragma once

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "Syntax.h"
#include "Text.h"
#include <Windows.h>

class LogListWindow;

class UIApplication : public Application {

public:

    UIApplication(HINSTANCE hInstance, wstring_view iniFileName);
    virtual ~UIApplication();

    HINSTANCE GetInstanceHandle() const;

    wstring GetModuleFilePath(wstring_view relativeFilePath) const override;
    ApplicationSettingsSection* GetSettingsSection(wstring_view name) override;

    void SetLogListWindow(LogListWindow* logListWindow);

    void SetClipboardText(wstring_view text);


protected:
    wstring GetText(Text::TextID textID) const override;

private:
    HINSTANCE hInstance;
    wstring settingsFilePath;

    LogListWindow* logListWindow = nullptr;
};


