/*
** WorkspaceLogic.cpp
**
** Load and save workspace.
*/

#include "WorkspaceLogic.h"

#include "Application.h"
#include "Byte.h"
#include "ByteArray.h"
#include "FileIO.h"
#include "String.h"
#include "FileInputStream.h"
#include "OutputStream.h"
#include "SegmentList.h"
#include "Workspace.h"
#include "Workspace1X.h"
#include "XML.h"
#include "FileType.h"
#include "SegmentListInserter.h"
#include "systems/ComputerSystem.h"
#include <sstream>

extern std::unique_ptr<Application> g_Application;

bool WorkspaceLogic::Load(Workspace& workspace, wstring_view filePath) {
    workspace.Init();

    ::g_Application->SendInfoMessageWithID(IDS_LOG_OPEN_WORK, filePath);

    constexpr auto magicSize = WORKSPACE_MAGIC_SIZE;
    auto byteArray = ByteArray(magicSize);
    FileInputStream inputStream;
    try {

        inputStream.OpenFile(filePath, std::ios::binary);
        inputStream.Read(byteArray.get(), byteArray.size());
    }
    catch (const IOException& ex) {
        ::g_Application->SendErrorMessageWithException(ex);
        return false;

    }
    const auto magic = byteArray.stringAt(0, byteArray.size());

    workspace.BeginUpdate();
    try {
        if (magic == WORKSPACE_MAGIC10) {
            Workspace1X::Load10(workspace, inputStream);
        }
        else if (magic == WORKSPACE_MAGIC14) {

            Workspace1X::Load14(workspace, inputStream);
        }
        else {
            const auto error = XML::Load(workspace, L"Workspace", filePath);
            if (error != XML::Error::XML_SUCCESS) { // TODO: Use input stream instead and throw IOException in XML::Load
                throw IOException(wstringstream() << "Error " << error << " while reading XML stream of \"" << filePath << "\".");
            }
        }
        workspace.SetFilePath(filePath);

        const auto& segmentList = workspace.GetSegmentList();
        const auto count = segmentList->GetCount();
        if (count > 0) {
            segmentList->SetSelectedIndex(0);
        }
        workspace.EndUpdate();
        return true;
    }
    catch (const IOException& ex) {
        workspace.Init();
        ::g_Application->SendErrorMessageWithID(IDS_ERR_BAD_WORKSPACE);
        ::g_Application->SendErrorMessageWithException(ex);
    }

    return false;
}

bool WorkspaceLogic::Save(Workspace& workspace, wstring_view filePath, Workspace::Format format) {
    ::g_Application->SendInfoMessageWithID(IDS_LOG_SAVE_WORKSPACE_FILE, filePath);
    workspace.SetFilePath(filePath);

    OutputStream* outputStream = nullptr;
    auto saved = false;
    try {

        switch (format) {
        case Workspace::Format::WORKSPACE14: {
            outputStream = OutputStream::OpenFile(filePath, Encoding::UNKNOWN);
            Workspace1X::Save14(workspace, *outputStream);
            saved = true;

            break;
        }
        case Workspace::Format::WORKSPACE36: {
            outputStream = OutputStream::OpenFile(filePath, Encoding::UTF8);
            const auto result = XML::Save(workspace, L"Workspace", *outputStream);

            if (result != XML::Error::XML_SUCCESS) {
                throw IOException(String::utf8_to_wstring(XML::Document::ErrorIDToName(result)));
            }


            saved = true;
            break;
        }
        default:
            throw IOException(L"Unkown workspace format");
            break;
        }

        delete outputStream;
    }
    catch (const IOException& ex) {
        if (outputStream != nullptr) {
            delete outputStream;

        }
        ::g_Application->SendErrorMessageWithException(ex);
        saved = false;
    }

    return saved;
}


bool WorkspaceLogic::AddFile(Workspace& workspace, FileType fileType, wstring_view filePath) {
    FileInputStream fis;
    try {
        const auto fileSize = fis.OpenFile(filePath, std::ios::binary);

        return AddFile(workspace, fileType, fis, fileSize);
    }
    catch (const IOException& ex) {
        ::g_Application->SendErrorMessageWithException(ex);
        return false;
    }

}

bool WorkspaceLogic::AddFile(Workspace& workspace, FileType fileType, FileInputStream& inputStream, FileIO::FILE_SIZE fileSize) {
    auto segmentLististInserter = workspace.GetSegmentList()->CreateInserter();
    const auto& computerSystem = workspace.GetComputerSystem();
    try {
        computerSystem->ReadFile(fileType, inputStream, fileSize, *segmentLististInserter);
    }
    catch (const IOException& ex) {
        segmentLististInserter->Cancel();
        ::g_Application->SendErrorMessageWithException(ex);
        return false;
    }

    segmentLististInserter->Apply();
    return true;
}

void WorkspaceLogic::AddRawSegment(Workspace& workspace, const ByteSequence& buffer, Memory::offset offset, Memory::size size, Memory::address address) {
    auto segmentlistInserter = workspace.GetSegmentList()->CreateInserter();
    try {
        const auto& segment = segmentlistInserter->InsertSegment();
        segment->wBegin = address;
        segment->wEnd = address + size - 1;

        segment->CreateMemoryBlockFromBeginToEnd();

        segment->bBinary = true;
        segment->SetData(0, buffer, offset, size);

        segmentlistInserter->Apply();
    }
    catch (const IOException& ex) {
        segmentlistInserter->Cancel();
        ::g_Application->SendErrorMessageWithID(IDS_ERR_READING_FILE, ex.GetText());
    }
}
