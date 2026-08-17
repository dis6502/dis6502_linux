#include "ApplicationSettingsSection.h"
#include "PlatformCompat.h"
#include <vector>

#include <fstream>
#include <sstream>
#include <algorithm>

namespace {

wstring Trim(wstring s) {
    const wchar_t* ws = L" \t\r\n";
    auto start = s.find_first_not_of(ws);
    if (start == wstring::npos) return L"";
    auto end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

// Minimal INI reader: returns the value for [section]/key, or defaultValue.
wstring ReadIniValue(const wstring& filePath, const wstring& section, const wstring& key, const wstring& defaultValue) {
    std::wifstream file(WStringToUtf8Compat(filePath));
    if (!file.is_open()) return defaultValue;

    wstring line;
    wstring currentSection;
    while (std::getline(file, line)) {
        wstring trimmed = Trim(line);
        if (trimmed.empty() || trimmed[0] == L';' || trimmed[0] == L'#') continue;

        if (trimmed.front() == L'[' && trimmed.back() == L']') {
            currentSection = trimmed.substr(1, trimmed.size() - 2);
            continue;
        }
        if (currentSection != section) continue;

        auto eq = trimmed.find(L'=');
        if (eq == wstring::npos) continue;
        wstring k = Trim(trimmed.substr(0, eq));
        if (k == key) {
            return Trim(trimmed.substr(eq + 1));
        }
    }
    return defaultValue;
}

// Minimal INI writer: rewrites the whole file with the key set/updated
// under [section], preserving any other sections/keys already present.
void WriteIniValue(const wstring& filePath, const wstring& section, const wstring& key, const wstring& value) {
    std::vector<wstring> lines;
    {
        std::wifstream in(WStringToUtf8Compat(filePath));
        wstring line;
        while (std::getline(in, line)) lines.push_back(line);
    }

    bool inSection = false;
    bool wroteKey = false;
    int sectionStart = -1, sectionEnd = -1;

    for (size_t i = 0; i < lines.size(); i++) {
        wstring trimmed = Trim(lines[i]);
        if (!trimmed.empty() && trimmed.front() == L'[' && trimmed.back() == L']') {
            wstring name = trimmed.substr(1, trimmed.size() - 2);
            if (inSection) { sectionEnd = static_cast<int>(i); break; }
            if (name == section) { inSection = true; sectionStart = static_cast<int>(i); }
            continue;
        }
        if (inSection) {
            auto eq = trimmed.find(L'=');
            if (eq != wstring::npos && Trim(trimmed.substr(0, eq)) == key) {
                lines[i] = key + L"=" + value;
                wroteKey = true;
            }
        }
    }
    if (inSection && sectionEnd == -1) sectionEnd = static_cast<int>(lines.size());

    if (sectionStart == -1) {
        // Section didn't exist: append it.
        lines.push_back(L"[" + section + L"]");
        lines.push_back(key + L"=" + value);
    } else if (!wroteKey) {
        lines.insert(lines.begin() + sectionEnd, key + L"=" + value);
    }

    std::wofstream out(WStringToUtf8Compat(filePath));
    for (auto& l : lines) out << l << L"\n";
}

} // namespace

ApplicationSettingsSection::ApplicationSettingsSection(wstring_view settingsFilePath, wstring_view name) {
    this->settingsFilePath = settingsFilePath;
    this->name = name;
}

void ApplicationSettingsSection::GetString(wstring_view keyName, wstring& value, wstring_view defaultValue) const {
    value = ReadIniValue(settingsFilePath, name, wstring(keyName), wstring(defaultValue));
}

void ApplicationSettingsSection::WriteString(wstring_view keyName, wstring_view value) const {
    WriteIniValue(settingsFilePath, name, wstring(keyName), wstring(value));
}

void ApplicationSettingsSection::GetUnsignedInt(wstring_view keyName, unsigned int& value, unsigned int defaultValue) const {
    wstring text;
    GetString(keyName, text, std::to_wstring(defaultValue));
    try {
        value = static_cast<unsigned int>(std::stoul(text));
    } catch (...) {
        value = defaultValue;
    }
}
