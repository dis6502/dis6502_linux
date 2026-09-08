#include "MRUList.h"

#include "Application.h"
#include "ApplicationSettingsSection.h"
#include "FileIO.h"
#include "FileType.h"
#include "MRUEntry.h"
#include "StringUtility.h"

extern Application* g_Application;

MRUList::MRUList(wstring_view sectionName, size_t maxEntries) :
    settingsSection(g_Application->GetSettingsSection(sectionName)),
    maxEntries(maxEntries) {};

MRUList::~MRUList() noexcept(false) {
    Clear();
}

void MRUList::Clear() {
    for (auto& entry : entries) {
        delete entry;
    }
    entries.clear();
}

void MRUList::AddFile(wstring_view filePath, const FileType fileType) {

    AddEntryAndReorder(MRUEntry::Create(filePath, fileType));
}

void MRUList::AddEntryAndReorder(const gsl::not_null<MRUEntry*> newEntry) {
    for (auto it = entries.begin(); it != entries.cend(); it++) {
        auto& entry = *it;

        if (String::EqualsIgnoreCase(entry->GetFilePath(), newEntry->GetFilePath())) {
            entries.erase(it);
            break;
        }
    }

    if (entries.size() >= maxEntries) {
        entries.pop_back();
    }

    entries.insert(entries.cbegin(), newEntry);
}

const std::vector<gsl::not_null<const MRUEntry*>> MRUList::GetEntries() const {
    return entries;
}

wstring MRUList::GetLastFilePath(FileType fileType) const {

    // Get first entry.
    for (auto& entry : entries) {
        if (entry->GetFileType() == fileType) {
            return entry->GetFilePath();
        }
    }
    return FileIO::EMPTY_FILE_PATH;
}


void MRUList::Load() {
    Clear();

    for (size_t i = 1; i <= maxEntries; i++) {

        auto suffix = std::to_wstring(i);
        wstring filePath;
        settingsSection->GetString(L"FilePath_" + suffix, filePath);
        if (filePath.empty()) {
            continue;
        }

        wstring fileTypeString;
        settingsSection->GetString(L"FileType_" + suffix, fileTypeString);
        if (fileTypeString.empty()) {
            continue;
        }

        const auto fileType = FileTypeFactory::GetInfo(fileTypeString).fileType;

        if (fileType != FileType::UNKNOWN_FILE) {
            entries.push_back(MRUEntry::Create(filePath, fileType));
        }
    }
}

void MRUList::Save() const {
    int index = 1;

    for (const auto& entry : entries) {
        auto suffix = std::to_wstring(index);
        settingsSection->WriteString(L"FileType_" + suffix, entry->GetFilePath());
        settingsSection->WriteString(L"FileType_" + suffix, FileTypeFactory::GetInfo(entry->GetFileType()).key);
        index++;
    }
}