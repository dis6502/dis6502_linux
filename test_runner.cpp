
#include <climits>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <stdexcept>
#include <unistd.h>
#include <utility>

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "MainTest.h"
#include "ui/Console.h"

extern Application* g_Application;

// Minimal Application for the Linux test runner: there's no Win32
// GetModuleFileName/GetPrivateProfileString here, so paths are resolved
// relative to the binary's own location via /proc/self/exe, and settings
// are kept in memory only (the test suite never relies on persisted ini
// values).
class TestApplication : public Application {
public:
    wstring GetModuleFilePath(wstring_view relativeFilePath) const override {
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len == -1) {
            throw std::runtime_error("Cannot determine module file path");
        }
        buffer[len] = '\0';

        auto result = std::filesystem::path(buffer).parent_path();
        if (!relativeFilePath.empty()) {
            result /= std::filesystem::path(wstring(relativeFilePath));
        }
        return result.wstring();
    }

    ApplicationSettingsSection* GetSettingsSection(wstring_view name) override {
        wstring nameString(name);
        auto it = settingsSections.find(nameString);
        if (it == settingsSections.end()) {
            settingsSections.insert(std::pair(nameString, ApplicationSettingsSection(*this, L"", name)));
        }
        return &settingsSections.at(nameString);
    }
};

int main() {
    TestApplication application;
    g_Application = &application;

    Console console;
    MainTest mainTest(console);
    mainTest.Execute(MainTest::TestMode::FAST);

    return 0;
}
