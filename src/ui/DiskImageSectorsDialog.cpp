#include "Byte.h"
#include "Dialog.h"
#include "DiskImage.h"
#include "DiskImageSectorsDialog.h"
#include "EditControl.h"
#include "ListBox.h"
#include "Memory.h"
#include "MemoryInspectorControl.h"
#include "Resource.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "TextLabel.h"
#include "Window.h"
#include "Word.h"
#include "gsl/pointers"
#include "Font.h"
#include <cwchar>
#include <list>
#include <memory>
#include <utility>
#include <Windows.h>

DiskImageSectorsDialog::DiskImageSectorsDialog(const Window& parentWindow, Font* computerFont) : Dialog(parentWindow, L"OPENDISKIMAGESECTORSBOX") {
    this->computerFont = computerFont;

    diskImageFilePath = L"";
    wCurrentSectorNumber = 0;
    nCurrentSectorSize = -1;
}

Window::INT_PTR DiskImageSectorsDialog::Show(wstring_view diskImageFilePath) {
    this->diskImageFilePath = diskImageFilePath;

    DiskImage::GetInfo(diskImageFilePath, diskImageInfo);

    sector.filePath = diskImageFilePath;
    sector.wSectorSize = diskImageInfo.wDensity;

    wCurrentSectorNumber = 0;
    nCurrentSectorSize = -1;
    ClearItems();

    return ShowDialogBox();
}

bool DiskImageSectorsDialog::ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) {
    word wAddr;
    ITEM_LINE szItemLine;
    int nItemCount;
    bool bSelected;

    switch (message) {
    case WM_INITDIALOG:
        CreateControls();
        ReadAndDisplaySector(1);
        return true;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            const auto& sectorsListBox = GetListBox(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST);
            nItemCount = sectorsListBox.GetCount();

            for (int iItem = 0; iItem < nItemCount; iItem++) {
                auto item = std::make_unique<Item>();

                auto itemText = sectorsListBox.GetStringAtIndex(iItem);
                swscanf(itemText.c_str(), ITEM_LINE_FORMAT, &item->wSector, &item->wAddr, &item->wBegin, &item->wSize);

                items.push_back(std::move(item));
            }

            return EndDialogBox(TRUE);
        }

        case IDCANCEL:
            return EndDialogBox(FALSE);

        case IDC_DISK_IMAGE_SECTORS_ADDRESS:
            if (HIWORD(wParam) == EN_CHANGE) {
                auto hexAddr = GetEditControl(IDC_DISK_IMAGE_SECTORS_ADDRESS).GetText(); // TODO Have IsAddressValid
                const bool bValid = (swscanf(hexAddr.c_str(), L"%hX", &wAddr) == 1);
                GetButton(IDC_DISK_IMAGE_SECTORS_ADD_SECTOR).SetEnabled(bValid);
            }

            return true;

        case IDC_DISK_IMAGE_SECTORS_ADD_SECTOR: {
            auto hexAddr = GetEditControl(IDC_DISK_IMAGE_SECTORS_ADDRESS).GetText();

            const bool valid = (swscanf(hexAddr.c_str(), L"%hX", &wAddr) == 1);
            if (valid) {
                Memory::offset wBegin = 0, wEnd = 0;
                memoryInspectorControl->GetSelection(wBegin, wEnd, true);

                const Memory::size wSize = wEnd - wBegin + 1;
                String::Printf(szItemLine, ITEM_LINE_FORMAT, wCurrentSectorNumber, wAddr, wBegin, wSize);

                GetListBox(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST).AddString(szItemLine);

                GetButton(IDOK).SetEnabled(true);

                wAddr += wSize;

                GetEditControl(IDC_DISK_IMAGE_SECTORS_ADDRESS).SetAddress(wAddr);

                if (wCurrentSectorNumber != diskImageInfo.wSectors) {
                    ReadAndDisplaySector(wCurrentSectorNumber + 1);
                }
            }

            return true;
        }

        case IDC_DISK_IMAGE_SECTORS_REMOVE_SECTOR: {
            auto& sectorsListBox = GetListBox(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST);
            nItemCount = sectorsListBox.GetCount();
            for (int iItem = nItemCount - 1; iItem >= 0; iItem--) {
                if (sectorsListBox.IsSelectedIndex(iItem)) {
                    sectorsListBox.DeleteStringAtIndex(iItem);
                }
            }

            GetButton(IDC_DISK_IMAGE_SECTORS_REMOVE_SECTOR).SetEnabled(false);
            GetButton(IDOK).SetEnabled(sectorsListBox.GetCount() > 0);

            return true;
        }

        case IDC_DISK_IMAGE_SECTORS_SECTORS_LIST: {
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                const auto& sectorsListBox = GetListBox(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST);
                nItemCount = sectorsListBox.GetCount();
                bSelected = false;

                for (int iItem = 0; !bSelected && iItem < nItemCount; iItem++)
                    if (sectorsListBox.IsSelectedIndex(iItem)) {
                        bSelected = true;
                        break;
                    }

                GetButton(IDC_DISK_IMAGE_SECTORS_REMOVE_SECTOR).SetEnabled(bSelected);
            }

            return true;
        }
        }
        break;

    case WM_HSCROLL:
        Scroll(wParam);
        return true;
    }

    return false;
}

void DiskImageSectorsDialog::CreateControls() {

    GetTextLabel(IDC_DISK_IMAGE_SECTORS_DISK_IMAGE_FILE_PATH).SetText(diskImageFilePath);
    SetScrollRange(GetDlgItem(hDlg, IDC_DISK_IMAGE_SECTORS_SECTOR_SCROLL), SB_CTL, 1, diskImageInfo.wSectors, false);

    GetEditControl(IDC_DISK_IMAGE_SECTORS_ADDRESS).Clear();

    memoryInspectorControl = std::make_unique<MemoryInspectorControl>(*this, IDC_DISK_IMAGE_SECTORS_SECTOR_DUMP);
    memoryInspectorControl->BindControl();
    memoryInspectorControl->SetFont(computerFont);
    memoryInspectorControl->SetNumberOfBytesPerLine(16);

    //memoryInspectorControl->SetBuffer(lpFileBuffer, (WORD)fileSize); TOD Set buffer
    //memoryInspectorControl->ClearSelection();

    GetTextLabel(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST_HEADER).SetFont(computerFont);
    GetListBox(IDC_DISK_IMAGE_SECTORS_SECTORS_LIST).SetFont(computerFont);

    GetButton(IDC_DISK_IMAGE_SECTORS_REMOVE_SECTOR).SetEnabled(false);
    GetButton(IDC_DISK_IMAGE_SECTORS_ADD_SECTOR).SetEnabled(false);
    GetButton(IDOK).SetEnabled(false);
}

void DiskImageSectorsDialog::DeleteControls() {
    memoryInspectorControl.reset();;
}

void DiskImageSectorsDialog::Scroll(WPARAM wParam) {
    auto sectorNumber = wCurrentSectorNumber;

    switch (LOWORD(wParam)) {
    case SB_TOP:
        sectorNumber = 1;
        break;

    case SB_BOTTOM:
        sectorNumber = diskImageInfo.wSectors;
        break;

    case SB_PAGEUP:
        if (sectorNumber > 18) {
            sectorNumber -= 18;
        }
        else {
            sectorNumber = 1;
        }
        break;

    case SB_PAGEDOWN:
        if (sectorNumber < diskImageInfo.wSectors - 18) {
            sectorNumber += 18;
        }
        else {
            sectorNumber = diskImageInfo.wSectors;
        }
        break;

    case SB_LINEUP:
        if (sectorNumber > 1) {
            sectorNumber--;
        }
        break;

    case SB_LINEDOWN:
        if (sectorNumber < diskImageInfo.wSectors) {
            sectorNumber++;
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        sectorNumber = HIWORD(wParam);
        break;

    default:
        return;
    }

    ReadAndDisplaySector(sectorNumber);
}

byte* DiskImageSectorsDialog::ReadSector(word wSector, int& nSectorSize) {
    DiskImage::ReadAbsoluteSector(sector, wSector, nSectorSize);

    return sector.cSectorData.get();
}

std::list<gsl::not_null<DiskImageSectorsDialog::Item*>> DiskImageSectorsDialog::GetItems() {
    std::list<gsl::not_null<Item*>> result;
    for (const auto& item : items) {
        result.push_back(item.get());
    }
    return result;
}

void DiskImageSectorsDialog::ClearItems() {
    items.clear();
}

void DiskImageSectorsDialog::ReadAndDisplaySector(word wSector) {
    if (wSector != wCurrentSectorNumber) {
        wCurrentSectorNumber = wSector;

        GetEditControl(IDC_DISK_IMAGE_SECTORS_SECTOR_NUMBER).SetNumber(wCurrentSectorNumber);
        SetScrollPos(GetDlgItem(hDlg, IDC_DISK_IMAGE_SECTORS_SECTOR_SCROLL), SB_CTL, wCurrentSectorNumber, true);

        ReadSector(wCurrentSectorNumber, nCurrentSectorSize);

        memoryInspectorControl->SetBuffer(sector.cSectorData.get(), nCurrentSectorSize);
        memoryInspectorControl->Refresh();

        memoryInspectorControl->ClearSelection();
    }
}
