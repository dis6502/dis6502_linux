#include <Windows.h>
#include "FileSystemLogic.h"

#include "Application.h"
#include "DefaultFolders.h"
#include "FileType.h"
#include "String.h"
#include "Text.h"
#include "Window.h"

extern std::unique_ptr<Application> g_Application;

void FileSystemLogic::SetDefaultFolders(DefaultFolders& defaultFolders) {
    this->defaultFolders = &defaultFolders;
}

void FileSystemLogic::SplitFilePath(wstring_view filePath, wstring& initialFolderPath, wstring& resultFilePath, FolderType folderType) {
    // Derive initial directory from the filePath.
    auto const idx = filePath.find_last_of(L"\\");
    if (idx != wstring::npos) {
        initialFolderPath = filePath.substr(0, idx);
        resultFilePath = filePath.substr(idx + 1);
    }
    else {
        const auto defaultPath = defaultFolders->GetFolderPath(folderType);
        if (!defaultPath.empty()) {
            initialFolderPath = defaultPath;
        }
        resultFilePath = filePath;
    }
}

FileSystemLogicResult FileSystemLogic::GetOpenFileName(const Window& parentWindow, wstring_view filePath, FileType fileType) {

    if (defaultFolders == nullptr) {
        throw std::runtime_error("Default folders not set");
    }

    auto fti = FileTypeFactory::GetInfo(fileType);
    auto title = Text::Format(IDS_FILE_SYSTEM_LOGIC_OPEN_FILE_TITLE, fti.text);
    auto result = this->GetOpenFileName(parentWindow, filePath, title, fti.GetFilterPattern(), fti.folderType);
    return result;
}

FileSystemLogicResult FileSystemLogic::GetOpenFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType) {
    wstring initialFolderPath = L".";
    wstring resultFilePath;

    if (defaultFolders == nullptr) {
        throw std::runtime_error("Default folders not set");
    }

    SplitFilePath(filePath, initialFolderPath, resultFilePath, folderType);
    resultFilePath.resize(FileIO::FILE_PATH_SIZE);

    // Create working copies.
    wchar_t szFileTitle[1] = { 0 };
    auto initialFolderPathString = initialFolderPath;
    auto titleString = wstring(title);

    // Fill the structure.
    OPENFILENAME of = {};
    of.lStructSize = sizeof(OPENFILENAME);
    of.hwndOwner = parentWindow.GetHWnd();
    of.hInstance = ::g_Application->GetInstanceHandle();
    of.lpstrFilter = filterPattern.data();
    of.lpstrCustomFilter = nullptr;
    of.nMaxCustFilter = 0L;
    of.nFilterIndex = 1L;
    of.lpstrFile = resultFilePath.data();
    of.nMaxFile = resultFilePath.capacity();
    of.lpstrFileTitle = szFileTitle;
    of.nMaxFileTitle = sizeof(szFileTitle);
    of.lpstrInitialDir = initialFolderPathString.data();
    of.lpstrTitle = titleString.data();
    of.Flags = OFN_FILEMUSTEXIST;
    of.nFileOffset = 0;
    of.nFileExtension = 0;
    of.lpstrDefExt = nullptr;
    of.lCustData = 0L;
    of.lpfnHook = nullptr;
    of.lpTemplateName = nullptr;

    FileSystemLogicResult result;
    result.success = false;
    result.filePath = L"";

    /*
    ** The dialog box is opened. If it returns true, it means that
    ** the user has choosen a file and we put into the result.
    */
    if (::GetOpenFileName(&of)) {
        result.filePath = wstring(of.lpstrFile);
        result.success = true;
    }

    return result;
}

FileSystemLogicResult FileSystemLogic::GetSaveFileName(const Window& parentWindow, wstring_view filePath, FileType fileType) {

    if (defaultFolders == nullptr) {
        throw std::runtime_error("Default folders not set");
    }

    auto ftr = FileTypeFactory::GetInfo(fileType);
    auto title = Text::Format(IDS_FILE_SYSTEM_LOGIC_SAVE_FILE_TITLE, ftr.text);

    return GetSaveFileName(parentWindow, filePath, title, ftr.GetFilterPattern(), ftr.folderType, ftr.defaultExtension);
}

/*
** SaveFileName is used to open a standard dialog box to choose a file for saving.
** It accepts a filename, the dialog title resource, the filters and folder type.
**
** This function returns result.success=true if the user has selected a file. Otherwise, it returns result.success=false.
*/
FileSystemLogicResult FileSystemLogic::GetSaveFileName(const Window& parentWindow, wstring_view filePath, wstring_view title, FileTypeInfo::FilterPattern filterPattern, FolderType folderType, wstring_view defaultExtension) {
    constexpr DWORD dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_NOREADONLYRETURN | OFN_OVERWRITEPROMPT;
    wstring initialFolderPath = L".";
    wstring resultFilePath;

    if (defaultFolders == nullptr) {
        throw std::runtime_error("Default folders not set");
    }

    SplitFilePath(filePath, initialFolderPath, resultFilePath, folderType);
    resultFilePath.resize(FileIO::FILE_PATH_SIZE);

    // The default extension in the API is without leading dot.
    auto defaultExtensionString = wstring(defaultExtension);
    if (defaultExtensionString.starts_with(L".")) {
        defaultExtensionString.erase(defaultExtensionString.begin());
    }

    // Create working copies.
    wchar_t szFileTitle[1] = { 0 };
    auto initialFolderPathString = initialFolderPath;
    auto titleString = wstring(title);

    OPENFILENAME of = {};
    of.lStructSize = sizeof(OPENFILENAME);
    of.hwndOwner = parentWindow.GetHWnd();
    of.hInstance = ::g_Application->GetInstanceHandle();
    of.lpstrFilter = filterPattern.data();
    of.lpstrCustomFilter = nullptr;
    of.nMaxCustFilter = 0L;
    of.nFilterIndex = 1L;
    of.lpstrFile = resultFilePath.data();
    of.nMaxFile = resultFilePath.capacity();
    of.lpstrFileTitle = szFileTitle;
    of.nMaxFileTitle = sizeof(szFileTitle);
    of.lpstrInitialDir = initialFolderPathString.data();
    of.lpstrTitle = titleString.data();
    of.Flags = dwFlags;
    of.nFileOffset = 0;
    of.nFileExtension = 0;
    of.lpstrDefExt = defaultExtensionString.c_str(); // ##c_str() OK
    of.lCustData = 0L;
    of.lpfnHook = nullptr;
    of.lpTemplateName = nullptr;

    FileSystemLogicResult result;
    result.success = false;
    result.filePath = L"";

    /*
    ** The dialog box is opened. If it returns true, it means that
    ** the user has choosen a file and we put into the result.
    */
    if (::GetSaveFileName(&of)) {
        result.filePath = wstring(of.lpstrFile);
        result.success = true;
    }

    return result;
}
