#pragma once

#include "Syntax.h"
#include <gsl/gsl>

enum class FileType;

class MRUEntry {
public:

    static gsl::not_null<MRUEntry*> Create(wstring_view filePath, FileType fileType);

	wstring GetFilePath() const;
	FileType GetFileType() const;

private:
	const wstring filePath;
	const FileType fileType;

    MRUEntry(wstring_view filePath, FileType fileType);
};