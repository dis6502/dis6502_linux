#include "Application.h"
#include "Byte.h"

#include "ByteArray.h"
#include "CommonIO.h"
#include "Dialog.h"
#include "EditControl.h"
#include "FileDialogs.h"
#include "FileIO.h"
#include "FileType.h"
#include "Memory.h"
#include "Resource.h"
#include "Segment.h"
#include "SegmentWriteBootDiskDialog.h"
#include "Syntax.h"
#include "gsl/pointers"
#include "systems/atari800/AtariDiskImage.h"
#include "systems/atari800/AtariDOS.h"
#include "Window.h"
#include <algorithm>
#include <cwchar>
#include <memory>
#include <string.h>
#include <Windows.h>

extern Application* g_Application;
extern std::unique_ptr<FileDialogs> g_FileDialogs;


SegmentWriteBootDiskDialog::SegmentWriteBootDiskDialog(const Window& parentWindow) : Dialog(parentWindow, L"WRITEBOOTBOX"), segment(nullptr), withInitAddress(false), initAddress(false) {}

void SegmentWriteBootDiskDialog::Show(gsl::not_null<const Segment*> segment, bool bWithInitAddress, Memory::address initAddress) {
    this->segment = segment;
    this->withInitAddress = bWithInitAddress;
    this->initAddress = initAddress;
    this->sector = ByteArray(128);
    ShowDialogBox();
}

bool SegmentWriteBootDiskDialog::ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) {
    switch (command) {
    case IDC_BOOTLOADADDR:
    case IDC_BOOTINITADDR: {
        if (HIWORD(wParam) == EN_CHANGE) {
            bool selected = false;

            if (GetEditControl(IDC_BOOTLOADADDR).HasText() &&
                GetEditControl(IDC_BOOTINITADDR).HasText()) {
                selected = true;
            }

            GetButton(IDOK).SetEnabled(selected);
        }

        return true;
    }

    case IDOK:
        return OnOK();

    case IDCANCEL:
        return OnCancel();

    default:
        break;
    }
    return false;
}

bool SegmentWriteBootDiskDialog::InitDialog() {
    GetEditControl(IDC_BOOTLOADADDR).SetAddress(segment->wBegin);

    if (withInitAddress) {
        GetEditControl(IDC_BOOTINITADDR).SetAddress(initAddress);
    }

    return true;
}

bool SegmentWriteBootDiskDialog::OnOK() {
    auto result = ::g_FileDialogs->ChooseSaveFileName(*this, L"", FileType::DISK_IMAGE_BOOT_SECTORS);
    if (result.success) {
        WriteBootDisk(hDlg, result.filePath);
        return EndDialogBox(true);
    }

    return true;
}

void SegmentWriteBootDiskDialog::WriteBootDisk(HWND hDlg, wstring_view filePath) {
    AtariFile Info;
    auto atariDisk = AtariDOS::OpenAtariDisk(filePath);
    switch (atariDisk->FindFirst(Info)) {
    case AtariError::NO_ENTRY_FOUND: // TODO: Error message
        break;

    case AtariError::OK: {
        Memory::word headerSize = 6;
        Memory::offset segmentOffset = 0;
        auto dataSize = segment->GetSize();
        const AtariDiskImage::SectorCount sectorCount = (dataSize + headerSize + 127) / 128;

        Memory::address wAddr = 0;
        Memory::address wInit = 0;

        auto hexAddr = GetEditControl(IDC_BOOTLOADADDR).GetText();
        swscanf(hexAddr.c_str(), L"%hX", &wAddr);
        hexAddr = GetEditControl(IDC_BOOTINITADDR).GetText();
        swscanf(hexAddr.c_str(), L"%hX", &wInit);

        sector.setAt(0, 0);
        sector.setAt(1, (byte)sectorCount); // Boot images can only have up to 255 sectors
        sector.setAt(2, (wAddr & 0xFF));
        sector.setAt(3, ((wAddr >> 8) & 0xFF));
        sector.setAt(4, (wInit & 0xFF));
        sector.setAt(5, (wInit >> 8) & 0xFF);

        for (AtariDiskImage::SectorNumber sectorNumber = 1; sectorNumber <= sectorCount; sectorNumber++) {
            const auto sectorSize = std::min<AtariDiskImage::SectorSize>(128 - headerSize, dataSize);
            auto p = sector.get() + headerSize;
            auto pSize = sector.size() - headerSize;
            memset(p, 0, (128 - headerSize) * sizeof(byte));
            segment->memoryBlock.GetData()->copyTo(p, 0, pSize, segmentOffset, sectorSize);

            AtariDiskImage::WriteAbsoluteSector(filePath, sectorNumber, sector);

            headerSize = 0;
            segmentOffset += sectorSize;
            dataSize -= sectorSize;
        }
        break;
    }

    case AtariError::DISK_NOT_FOUND:
    {
        IOException ioException(FileIO::FormatError(IDS_ERR_READING_FILE, filePath));
        ::g_Application->SendErrorMessageWithException(ioException);
    }
    break;

    default:
    {
        IOException ioException(FileIO::FormatError(IDS_ERR_READING_ATR, filePath));
        ::g_Application->SendErrorMessageWithException(ioException);
    }
    break;
    }
}
