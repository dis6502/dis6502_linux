#pragma once

#include "Syntax.h"
#include "FileIO.h"
#include "Memory.h"
#include "Workspace.h"

enum class FileType;
class FileInputStream;

class WorkspaceLogic {
public:
    WorkspaceLogic() = default;

    /*
    ** Load workspace from disk.
    */
    bool Load(Workspace& workspace, wstring_view filePath); // does not throw IOException

    /*
    **  Save workspace to disk.
    */
    bool Save(Workspace& workspace, wstring_view filePath, Workspace::Format format); // does not throw IOException

    bool AddFile(Workspace& workspace, FileType fileType, wstring_view filePath);
    bool AddFile(Workspace& workspace, FileType fileType, FileInputStream& inputStream, FileIO::FILE_SIZE  fileSize);
    void AddRawSegment(Workspace& workspace, const ByteSequence& buffer, Memory::offset offset, Memory::size size, Memory::address address);
};
