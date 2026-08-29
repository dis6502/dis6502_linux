#include <algorithm>

#include "Application.h"
#include "systems/atari800/AtariDOS.h"				// TODO: Move DOS/Disk handling and merge to ComputerSystem
#include "systems/atari800/DiskImageFileInputStream.h"  // TODO: Move DOS/Disk handling and merge to ComputerSystem
#include "systems/ComputerSystem.h"
#include "systems/ComputerSystemType.h"
#include "ByteSequence.h"
#include "DisassemblyResultFile.h"
#include "DiskImage.h"
#include "DiskImageExecutableFileDialog.h"
#include "DiskImageSectorsDialog.h"
#include "FileInputStream.h"
#include "FileIO.h"
#include "FileSystemLogic.h"
#include "FileType.h"
#include "Main.h"
#include "MainWindow.h"
#include "MemoryInspector.h"
#include "MRUController.h"
#include "RawFileDialog.h"
#include "Segment.h"
#include "Memory.h"
#include "SegmentList.h"
#include "SegmentListInserter.h"
#include "Window.h"
#include "Workspace.h"
#include "WorkspaceLogic.h"
#include "DisassemblyResultWriter.h"

#include "MainFile.h"

extern wstring binPath;
extern wstring diskPath;

extern std::unique_ptr<Application> g_Application;
extern std::unique_ptr<Workspace> g_Workspace;

extern std::unique_ptr<FileSystemLogic> g_FileSystemLogic;


MainFile::MainFile(Main& main, MRUController& mruController, WorkspaceLogic& workspaceLogic) : MainController(main), mruController(&mruController), workspaceLogic(&workspaceLogic)
{

}

void MainFile::OpenRawFile(const Window& parentWindow, wstring_view filePath, bool add) {
    if (!ConfirmOpen(IDS_LOG_OPEN_RAW_FILE, filePath, add)) {
        return;
    }
    binPath = filePath;

    RawFileDialog dialog(parentWindow, ::g_Workspace->GetResizedFont());
    if (dialog.Show(binPath)) {

        Memory::offset offset = 0;
        Memory::size size = 0;
        Memory::address address = 0;
        auto fileBuffer = dialog.GetFileBuffer();
        dialog.GetResult(offset, size, address);

        workspaceLogic->AddRawSegment(*g_Workspace, fileBuffer, offset, size, address);
    }
}

void MainFile::OpenExecutableFile(const Window& parentWindow, wstring_view filePath, bool add) {
    if (!ConfirmOpen(IDS_LOG_OPEN_EXECUTABLE_FILE, filePath, add)) {
        return;
    }

    binPath = filePath;

    ReadFile(FileType::EXECUTABLE_FILE, binPath);
}

void MainFile::OpenRomImageFile(const Window& parentWindow, wstring_view filePath, bool add) {
    if (!ConfirmOpen(IDS_LOG_OPEN_ROM_IMAGE_FILE, filePath, add)) {
        return;
    }

    binPath = filePath;

    ReadFile(FileType::ROM_IMAGE_FILE, binPath);
}

void MainFile::OpenCassetteImageFile(const Window& parentWindow, wstring_view filePath, bool add) {
    if (!ConfirmOpen(IDS_LOG_OPEN_CASSETTE_FILE, filePath, add)) {
        return;
    }

    binPath = filePath;

    ReadFile(FileType::CASSETTE_IMAGE_FILE, binPath);
}

void MainFile::OpenDiskImageExecutableFile(const Window& parentWindow, wstring_view filePath, bool add) {

    // Check if disk is DOS disk at all.
    AtariFile info;
    auto atariDisk = AtariDOS::OpenAtariDisk(filePath);
    switch (atariDisk->FindFirst(info)) {
    case AtariError::OK: {
        DiskImageExecutableFileDialog dialog(parentWindow, ::g_Workspace->GetResizedFont());
        if (dialog.Show(filePath)) {
            auto executableFilePath = dialog.GetExecutableFilePath();
            if (!ConfirmOpen(IDS_LOG_OPEN_DISK_IMAGE_EXECUTABLE_FILE, executableFilePath, filePath, add)) {
                return;
            }

            diskPath = filePath;
            binPath = executableFilePath;

            DiskImageFileInputStream fis;
            try {
                const auto fileSize = fis.OpenDiskImageFile(diskPath, binPath);
                if (fileSize == 0) {
                    throw IOException(L"File in disk image is empty");
                }
                workspaceLogic->AddFile(*g_Workspace, FileType::EXECUTABLE_FILE, fis, fileSize);

            }
            catch (const IOException& ex) {
                ::g_Application->SendErrorMessageWithException(ex);
                return;
            }
        }
        break;
    }

    case AtariError::DISK_NOT_FOUND:
        g_Application->SendFileErrorMessageWithID(IDS_FILE_IO_EX_OPENING_FILE_FOR_READ_ACCESS, diskPath); // TODO: Make IOException
        break;

    case AtariError::NO_ENTRY_FOUND:
        ::g_Application->SendErrorMessageWithID(IDS_ERR_NO_ATARI_FILE, diskPath);
        break;

    default:
        ::g_Application->SendErrorMessageWithID(IDS_ERR_READING_ATR, diskPath);
        break;
    }
}


void MainFile::OpenDiskImageBootSectors(const Window& parentWindow, wstring_view filePath, bool add) {
    if (!ConfirmOpen(IDS_LOG_OPEN_DISK_IMAGE_BOOT_SECTORS, filePath, add)) {
        return;
    }

    diskPath = filePath;
    ImgInfo Info;
    ImgRWPacket sector;
    DiskImage::GetInfo(diskPath, Info);

    if (!DiskImage::DisplayError(Info.nResult)) {
        sector.filePath = diskPath;
        sector.wSectorNumber = 1;
        sector.wSectorSize = 128;	// Atari Boot Sectors are always 128 bytes long

        DiskImage::ReadSector(sector);

        if (!DiskImage::DisplayError(Info.nResult)) {
            if (sector.cSectorData.at(1) == 0) {
                ::g_Application->SendErrorMessageWithID(IDS_ERR_NO_BOOT, diskPath);
            }
            else {
                auto segmentListInserter(g_Workspace->GetSegmentList()->CreateInserter());
                try {
                    const auto& segment = segmentListInserter->InsertSegment();

                    AddDiskImageBootSectorsSegment(*segment, *sector.cSectorData.GetSubSequence(0, sector.wSectorSize));
                    segmentListInserter->Apply();
                }
                catch (const IOException& ex) {
                    segmentListInserter->Cancel();
                    ::g_Application->SendErrorMessageWithException(ex);
                }
            }
        }
    }
}

void MainFile::OpenDiskImageSectors(const Window& parentWindow, wstring_view filePath, bool add) {

    ImgInfo Info;
    DiskImage::GetInfo(filePath, Info);

    if (!DiskImage::DisplayError(Info.nResult)) {
        DiskImageSectorsDialog dialog(parentWindow, ::g_Workspace->GetResizedFont());

        if (dialog.Show(filePath)) {
            auto items = dialog.GetItems();
            if (!ConfirmOpen(IDS_LOG_OPEN_DISK_IMAGE_SECTORS, std::to_wstring(items.size()), filePath, add)) {
                return;
            }

            diskPath = filePath;

            auto segmentlistInserter = *g_Workspace->GetSegmentList()->CreateInserter();
            try {
                for (const auto& item : items) {

                    const auto& segment = segmentlistInserter.InsertSegment();

                    segment->bBinary = true;
                    segment->wBegin = item->wAddr;
                    segment->wEnd = item->wAddr + item->wSize - 1;

                    segment->CreateMemoryBlockFromBeginToEnd();

                    int sectorSize = 0;
                    const auto sectorData = dialog.ReadSector(item->wSector, sectorSize);
                    ByteArray byteArray(sectorData, item->wSize);
                    segment->SetData(0, byteArray, item->wBegin, byteArray.size());

                }
                segmentlistInserter.Apply();
            }
            catch (const IOException& ex) {
                segmentlistInserter.Cancel();
                application->SendErrorMessageWithException(ex);
            }
        }
    }
}

void MainFile::OpenFileWithDialog(const Window& parentWindow, const FileType fileType, bool add) {
    auto filePath = mruController->GetLastFilePath(fileType);

    auto result = ::g_FileSystemLogic->GetOpenFileName(parentWindow, filePath, fileType);

    if (result.success) {
        mruController->AddFile(result.filePath, fileType);
        OpenFile(parentWindow, result.filePath, fileType, add);
    }
}

void MainFile::OpenFile(const Window& parentWindow, wstring_view filePath, const FileType fileType, bool add) {
    auto realFileType = fileType;

    if (realFileType == FileType::UNKNOWN_FILE) {
        try {
            realFileType = ::g_Workspace->GetComputerSystem()->GuessFileType(filePath);
        }
        catch (const IOException& ex) {
            g_Application->SendErrorMessageWithException(ex);
            return;
        }
    }

    if ((realFileType == FileType::UNKNOWN_FILE) && String::EqualsIgnoreCase(File(filePath).GetExtension(), L".wrk")) {
        realFileType = FileType::WORKSPACE_FILE;
    }

    if (realFileType == FileType::RAW_FILE) {
        mruController->AddFile(filePath, realFileType);
        OpenRawFile(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::EXECUTABLE_FILE) {
        mruController->AddFile(filePath, realFileType);
        OpenExecutableFile(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::ROM_IMAGE_FILE) {
        mruController->AddFile(filePath, realFileType);
        OpenRomImageFile(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::CASSETTE_IMAGE_FILE) {
        mruController->AddFile(filePath, realFileType);
        OpenCassetteImageFile(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::DISK_IMAGE_EXECUTABLE_FILE) {
        mruController->AddFile(filePath, realFileType);
        OpenDiskImageExecutableFile(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::DISK_IMAGE_BOOT_SECTORS) {
        mruController->AddFile(filePath, realFileType);
        OpenDiskImageBootSectors(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::DISK_IMAGE_SECTORS) {
        mruController->AddFile(filePath, realFileType);
        OpenDiskImageSectors(parentWindow, filePath, add);
    }
    else if (realFileType == FileType::WORKSPACE_FILE) {
        mruController->AddFile(filePath, realFileType);
        main->OpenWorkspaceFile(filePath.data());
    }
}

void MainFile::ReadFile(FileType fileType, wstring_view filePath) {
    workspaceLogic->AddFile(*g_Workspace, fileType, filePath);
}

bool MainFile::SaveWorkspaceFile(const Window& parentWindow, bool bSaveAs) {
    auto filePath = ::g_Workspace->GetFilePath();
    if (filePath.empty() || bSaveAs) {
        FileSystemLogicResult result = ::g_FileSystemLogic->GetSaveFileName(parentWindow, filePath, FileType::WORKSPACE_FILE);

        if (!result.success) {
            return false;
        }
        filePath = result.filePath;
    }

    mruController->AddFile(filePath, FileType::WORKSPACE_FILE);

    const bool saveResult = workspaceLogic->Save(*g_Workspace, filePath, Workspace::Format::WORKSPACE36);
    return saveResult;
}

void MainFile::SaveDisassemblyFiles(const Window& parentWindow) {
    const FileSystemLogicResult result = ::g_FileSystemLogic->GetSaveFileName(parentWindow, L"", FileType::DISASSEMBLY_FILE);

    if (result.success) {
        DisassemblyResultFile disassemblyResultFile;
        try {
            disassemblyResultFile.SaveListing(*::g_Workspace->GetConstDisassemblyResult(), *g_Workspace->GetConstProfile(), File(result.filePath));
        }
        catch (const IOException& ex) {
            g_Application->SendErrorMessageWithException(ex);
        }
    }
}

// TODO: Move to ComputerSystem
void MainFile::AddDiskImageBootSectorsSegment(Segment& segment, const ByteSequence& sectorsData) {

    binPath = L"BOOT";
    auto wSectors = sectorsData.at(1);
    Memory::word wSize = (wSectors * 128) - 6;

    segment.wBegin = sectorsData.at(2) + (sectorsData.at(3) * 256) + 6;
    segment.wEnd = segment.wBegin + wSize - 1;;

    segment.CreateMemoryBlockFromBeginToEnd();
    segment.bBinary = true;

    WORD wSecSize = std::min<WORD>(sectorsData.size() - 6, wSize); // TODO: conversion from 'size_t' to 'const _Ty', possible loss of data

    Memory::offset memoryInspectorPosition = 0;
    segment.SetData(memoryInspectorPosition, sectorsData, 6, wSecSize);

    wSize -= wSecSize;
    wSectors--;
    WORD wNumSec = 2;

    while (wSize && wSectors) {
        memoryInspectorPosition += wSecSize;
        wSecSize = std::min<WORD>(128, wSize);

        ImgRWPacket Sector;
        Sector.filePath = diskPath;
        Sector.wSectorNumber = wNumSec++;
        Sector.wSectorSize = 128;	// I assume that boot sectors are 128 bytes long.  TODO: Only the first 3 sectors have the guaranteed...

        DiskImage::ReadSector(Sector);

        if (!DiskImage::IsError(Sector.nResult)) {
            auto byteSequence = Sector.cSectorData.GetSubSequence(0, Sector.wSectorSize);
            segment.SetData(memoryInspectorPosition, *byteSequence, 0, byteSequence->size());

            wSize -= wSecSize;
            wSectors--;
        }
        else {
            break;
        }
    }
}

bool MainFile::ConfirmOpen(Text::TextID textID, wstring_view variable1, bool add) {
    return ConfirmOpen(textID, variable1, L"", add);
}

bool MainFile::ConfirmOpen(Text::TextID textID,wstring_view variable1, wstring_view variable2, bool add) {
    if (!add) {

        if (!main->PromptToClearWorkspace(IDS_MAIN_FILE_NEW_WORKSPACE_TITLE, IDS_MAIN_FILE_NEW_WORKSPACE_MESSAGE, !add)) {
            return false;
        }
    }
    ::g_Application->SendInfoMessageWithID(textID, variable1, variable2);
    return true;
}