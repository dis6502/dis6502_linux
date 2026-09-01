#pragma once

#include "FileIO.h"
#include "Memory.h"
#include "Syntax.h"
#include "Workspace.h"

class EquateListLogic;
enum class FileType;
class FileInputStream;
class ByteSequence;

class WorkspaceLogic {
public:
    WorkspaceLogic(EquateListLogic& equateListLogic);

    /*
    ** Load workspace from disk.
    */
    bool Load(Workspace& workspace, wstring_view filePath); // does not throw IOException

    /*
    **  Save workspace to disk.
    */
    bool Save(Workspace& workspace, wstring_view filePath, Workspace::Format format); // does not throw IOException

    /*
    ** Load the system equate for the computer system.
    */
    void LoadSystemEquates(Workspace& workspace);

    bool AddFile(Workspace& workspace, FileType fileType, wstring_view filePath);
    bool AddFile(Workspace& workspace, FileType fileType, FileInputStream& inputStream, FileIO::FILE_SIZE  fileSize);
    void AddRawSegment(Workspace& workspace, const ByteSequence& buffer, Memory::offset offset, Memory::size size, Memory::address address);

private:
    EquateListLogic* equateListLogic;

};
