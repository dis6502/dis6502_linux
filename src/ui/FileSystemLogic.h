#pragma once

#include "Syntax.h"
#include "FileType.h"
#include "FileIO.h"

class Application;
class DefaultFolders;
enum class FolderType;
class Window;



class FileSystemLogicResult {
public:
    bool success;
    wstring filePath;
};


class FileSystemLogic {
public:
    FileSystemLogic() = default;

    void SetDefaultFolders(DefaultFolders& defaultFolders);

    /*
    ** GetOpenFileName is used to open a standard dialog box to choose a file to open.
    ** It accepts a filename, and file type or the dialog title, the filters and the folder type.
    **
    ** This function returns true if the user has selected a file. Otherwise, it returns false.
    */
    FileSystemLogicResult GetOpenFileName(const Window& parentWindow, wstring_view filePath, FileType fileType);

    /*
    ** GetSaveFileName is used to open a standard dialog box to choose a file to save.
    ** It accepts a filename, and file type or the dialog title, the filters and the folder type.
    **
    ** This function returns true if the user has selected a file. Otherwise, it returns false.
    */
    FileSystemLogicResult GetSaveFileName(const Window& parentWindow, wstring_view filePath, FileType fileType);

private:
    DefaultFolders* defaultFolders = nullptr;

    void SplitFilePath(wstring_view filePath, wstring& initialFolderPath, wstring& resultFilePath, FolderType folderType);

    FileSystemLogicResult GetOpenFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType);
    FileSystemLogicResult GetSaveFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType, wstring_view defaultExtension);
};
