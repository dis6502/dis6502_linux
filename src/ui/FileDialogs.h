#pragma once

#include "FileType.h"
#include "Syntax.h"

class Application;
class DefaultFolders;
enum class FolderType;
class Window;


class FileDialogResult {
public:
    bool success;
    wstring filePath;
};


class FileDialogs {
public:
    FileDialogs() = default;

    void SetDefaultFolders(DefaultFolders& defaultFolders);

    /*
    ** GetOpenFileName is used to open a standard dialog box to choose a file to open.
    ** It accepts a filename, and file type or the dialog title, the filters and the folder type.
    **
    */
    FileDialogResult ChooseOpenFileName(const Window& parentWindow, wstring_view filePath, FileType fileType);

    /*
    ** GetSaveFileName is used to open a standard dialog box to choose a file to save.
    ** It accepts a filename, and file type or the dialog title, the filters and the folder type.
    **
    */
    FileDialogResult ChooseSaveFileName(const Window& parentWindow, wstring_view filePath, FileType fileType);

private:
    DefaultFolders* defaultFolders = nullptr;

    void SplitFilePath(wstring_view filePath, wstring& initialFolderPath, wstring& resultFilePath, FolderType folderType);

    FileDialogResult ChooseOpenFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType);
    FileDialogResult ChooseSaveFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType, wstring_view defaultExtension);
};
