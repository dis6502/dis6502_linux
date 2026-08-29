#pragma once

#include <list>

#include "Syntax.h"
#include "Dialog.h"
#include "DiskImage.h"
#include "MemoryInspectorControl.h"


class DiskImageSectorsDialog : public Dialog {
public:
    class Item;
    typedef Item* LP_ITEM;

    DiskImageSectorsDialog(const Window& parentWindow, HFONT hComputerFont);

    INT_PTR Show(wstring_view diskImageFilePath);

    std::list<gsl::not_null<Item*>> GetItems();

    byte* ReadSector(WORD wSector, int& nSectorSize);

protected:
    bool ProcessDialogMessage(UINT message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;

    void CreateControls() override;
    void DeleteControls() override;

private:
    // Set in constructor.
    HFONT hComputerFont;
    ImgInfo diskImageInfo;
    ImgRWPacket sector;

    // Set in Show()
    wstring diskImageFilePath;

    // Set in event handling
    std::unique_ptr<MemoryInspectorControl> memoryInspectorControl;

    WORD wCurrentSectorNumber;
    int nCurrentSectorSize;

    static constexpr auto ITEM_LINE_FORMAT = L"%6hu %04hX %02hX    %04hX";

    typedef wchar_t ITEM_LINE[80];
    std::list<gsl::not_null<Item*>> items;

    void ClearItems();

    void Scroll(WPARAM wParam);
    void ReadAndDisplaySector(WORD wSector);
};


class DiskImageSectorsDialog::Item {
public:
    WORD wSector;
    WORD wAddr;
    WORD wBegin;
    WORD wSize;
};
