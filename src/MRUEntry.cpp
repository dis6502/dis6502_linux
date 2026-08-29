#include "MRUEntry.h"

#include "FileType.h"

MRUEntry::MRUEntry(wstring_view filePath, FileType fileType) :
	filePath(filePath), fileType(fileType) {
}

gsl::not_null<MRUEntry*> MRUEntry::Create(wstring_view filePath, FileType fileType) {
  // The lifecycle is explicitly managed by the MRUList
#pragma warning(suppress: 26403) // Reset or explicitly delete an owner<T> pointer 'entry' (r.3).
#pragma warning(suppress: 26409) // Avoid calling new and delete explicitly, use std::make_unique<T> instead (r.11).
  return new MRUEntry(filePath, fileType);
}

wstring MRUEntry::GetFilePath() const {
	return filePath;
};

FileType MRUEntry::GetFileType() const {
	return fileType;
};
