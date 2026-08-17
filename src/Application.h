#pragma once

// Portable replacement for the original Win32 Application.h.
// The engine only ever calls this small surface (found by grepping every
// core .cpp for g_Application-> / Application:: usage):
//   SendErrorMessageWithID, SendInfoMessageWithID, SendErrorMessageWithException,
//   SendFileErrorMessageWithID, ThrowErrorMessageWithID,
//   GetSettingsSection, GetModuleFilePath, GetInstanceHandle, SetClipboardText
//
// The real UI layer (whatever replaces Win32 - e.g. an ImGui front end)
// should provide its own Application that implements this same interface,
// wiring SendErrorMessageWithID etc. into real dialogs/popups. This stub
// keeps the engine buildable and testable headlessly on Linux in the
// meantime: errors go to stderr, nothing pops up, nothing crashes.

#include "Syntax.h"
#include "Text.h"
#include "ApplicationSettingsSection.h"

#include <exception>
#include <map>
#include <memory>
#include <functional>

// Stand-in for HINSTANCE; the engine never dereferences this, it only
// forwards it to Win32-only APIs elsewhere (e.g. old Text::Get). Kept as
// an opaque pointer-sized type so signatures don't change.
using InstanceHandle = void*;

class Application {
public:
    Application();
    virtual ~Application();

    InstanceHandle GetInstanceHandle() const;
    wstring GetModuleFilePath(wstring_view relativeFilePath);

    ApplicationSettingsSection* GetSettingsSection(wstring_view name);

    void SetClipboardText(wstring_view text);

    // Optional hook the UI layer installs to actually reach the OS
    // clipboard (e.g. SDL_SetClipboardText) - a std::function rather
    // than a virtual method/subclass so core stays UI-toolkit-agnostic.
    // Defaults to a no-op (nullptr) if the UI layer never sets it.
    static std::function<void(wstring_view)> clipboardTextSetter;

    void SendInfoMessageWithID(Text::TextID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");
    void SendErrorMessageWithID(Text::TextID textID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"");
    void ThrowErrorMessageWithID(Text::TextID textID, wstring_view v0 = L"", wstring_view v1 = L"", wstring_view v2 = L"", wstring_view v3 = L"", wstring_view v4 = L"", wstring_view v5 = L"", wstring_view v6 = L"", wstring_view v7 = L"", wstring_view v8 = L"", wstring_view v9 = L"") const;
    void SendErrorMessageWithException(const std::exception& ex);
    void SendFileErrorMessageWithID(Text::TextID, wstring_view wfilePath);

protected:
    // Hook points for a real UI layer: override these instead of touching
    // engine call sites. Default behavior logs to stderr.
    virtual void ShowInfo(wstring_view message);
    virtual void ShowError(wstring_view message);

private:
    std::map<wstring, std::unique_ptr<ApplicationSettingsSection>> settingsSections;
};

extern std::unique_ptr<Application> g_Application;
