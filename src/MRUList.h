#pragma once

#include "Syntax.h"
#include <vector>
#include <gsl/gsl>

enum class FileType;
class ApplicationSettingsSection;
class MRUEntry;

class MRUList {
public:
	MRUList(wstring_view sectionName, size_t maxEntries);
	~MRUList() noexcept(false);

    MRUList(const MRUList& a) = delete;           // no copy constructor
    MRUList& operator=(const MRUList&) = delete;  // no copy assignment
    MRUList(MRUList&&) = delete;                  // no move constructor
    MRUList& operator=(MRUList&&) = delete;       // no move assignment

	void Clear();
	void AddFile(wstring_view filePath, FileType fileType);
	const std::vector<gsl::not_null<const MRUEntry*>>GetEntries() const ;
	wstring GetLastFilePath(FileType fileType) const;

	void Load();
	void Save() const;

private:
	ApplicationSettingsSection* settingsSection;
	size_t maxEntries;
	std::vector<gsl::not_null<const MRUEntry*>> entries;

	void AddEntryAndReorder(const gsl::not_null<MRUEntry*> newEntry);
};

