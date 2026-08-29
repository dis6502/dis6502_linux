#pragma once

#include <string_view>

#include "Syntax.h"

#include "MainController.h"

class Segment;
class FileInputStream;
enum class FileType;
class MRUController;
class Window;
class WorkspaceLogic;


class MainFile final : MainController {
public:
    MainFile(Main& main, MRUController& mruController, WorkspaceLogic& workspaceLogic);

    void OpenFileWithDialog(const Window& parentWindow, const FileType fileType, bool add);
    void OpenFile(const Window& parentWindow, wstring_view filePath, const FileType fileType, bool add);

    bool SaveWorkspaceFile(const Window& parentWindow, bool bSaveAs);
    void SaveDisassemblyFiles(const Window& parentWindow);

private:
    MRUController* mruController;
    WorkspaceLogic* workspaceLogic;

    void OpenCassetteImageFile(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenDiskImageBootSectors(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenDiskImageExecutableFile(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenDiskImageSectors(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenExecutableFile(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenRawFile(const Window& parentWindow, wstring_view filePath, bool add);
    void OpenRomImageFile(const Window& parentWindow, wstring_view filePath, bool add);

    void ReadFile(FileType fileType, wstring_view filePath);

    void AddDiskImageBootSectorsSegment(Segment& segment, const ByteSequence& byteSequence); // TODO Move and merge to ComputerSystem

    bool ConfirmOpen(Text::TextID textID, wstring_view variable, bool add);
    bool ConfirmOpen(Text::TextID textID, wstring_view variable1, wstring_view variable2, bool add);

};
