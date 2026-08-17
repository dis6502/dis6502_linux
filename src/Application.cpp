#include "Application.h"
#include "PlatformCompat.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>

std::unique_ptr<Application> g_Application = std::make_unique<Application>();

Application::Application() = default;
Application::~Application() = default;

InstanceHandle Application::GetInstanceHandle() const {
    return nullptr;
}

wstring Application::GetModuleFilePath(wstring_view relativeFilePath) {
    // Win32 build resolves this relative to the .exe's directory. On Linux
    // we resolve relative to the current executable's directory via
    // /proc/self/exe, falling back to the current working directory.
    std::error_code ec;
    auto exePath = std::filesystem::read_symlink("/proc/self/exe", ec);
    std::filesystem::path base = ec ? std::filesystem::current_path() : exePath.parent_path();
    auto full = base / std::filesystem::path(WStringToUtf8Compat(relativeFilePath));
    return Utf8ToWStringCompat(full.string());
}

ApplicationSettingsSection* Application::GetSettingsSection(wstring_view name) {
    auto key = wstring(name);
    auto it = settingsSections.find(key);
    if (it == settingsSections.end()) {
        auto settingsFilePath = GetModuleFilePath(L"dis6502.ini");
        auto section = std::make_unique<ApplicationSettingsSection>(settingsFilePath, name);
        auto* ptr = section.get();
        settingsSections[key] = std::move(section);
        return ptr;
    }
    return it->second.get();
}

std::function<void(wstring_view)> Application::clipboardTextSetter;

void Application::SetClipboardText(wstring_view text) {
    if (clipboardTextSetter) clipboardTextSetter(text);
}

void Application::ShowInfo(wstring_view message) {
    std::fwprintf(stderr, L"[info] %ls\n", wstring(message).c_str());
}

void Application::ShowError(wstring_view message) {
    std::fwprintf(stderr, L"[error] %ls\n", wstring(message).c_str());
}

void Application::SendInfoMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    ShowInfo(Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9));
}

void Application::SendErrorMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    ShowError(Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9));
}

void Application::ThrowErrorMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) const {
    throw std::runtime_error(WStringToUtf8Compat(Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)));
}

void Application::SendErrorMessageWithException(const std::exception& ex) {
    ShowError(Utf8ToWStringCompat(ex.what()));
}

void Application::SendFileErrorMessageWithID(Text::TextID textID, wstring_view wfilePath) {
    ShowError(Text::Format(textID, wfilePath));
}
