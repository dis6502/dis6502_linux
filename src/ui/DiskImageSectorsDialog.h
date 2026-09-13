#pragma once

#include "Byte.h"
#include "Dialog.h"
#include "DiskImage.h"
#include "MemoryInspectorControl.h"
#include "Syntax.h"
#include "Window.h"
#include "Word.h"
#include <gsl/pointers>
#include <list>
#include <memory>

class Font;

class DiskImageSectorsDialog : public Dialog {
public:
    class Item;
    typedef Item* LP_ITEM;

    DiskImageSectorsDialog(const Window& parentWindow, Font* computerFont);

    DialogFuncResult Show(wstring_view diskImageFilePath);

    std::list<gsl::not_null<Item*>> GetItems();

    byte* ReadSector(word wSector, int& nSectorSize);

protected:
    bool ProcessDialogMessage(MESSAGE message, WPARAM wParam, LPARAM lParam, INT_PTR& nResult) override;
    bool ProcessCommand(COMMAND command, WPARAM wParam, LPARAM lParam) override;
    bool InitDialog() override;
    bool OnOK() override;

    void CreateControls() override;
    void DeleteControls() override;

private:
    // Set in constructor.
    Font* computerFont;
    ImgInfo diskImageInfo;
    ImgRWPacket sector;

    // Set in Show()
    wstring diskImageFilePath;

    // Set in event handling
    std::unique_ptr<MemoryInspectorControl> memoryInspectorControl;

    word wCurrentSectorNumber;
    int nCurrentSectorSize;

    static constexpr auto ITEM_LINE_FORMAT = L"%6hu %04hX %02hX    %04hX";

    typedef wchar_t ITEM_LINE[80];
    std::list<std::unique_ptr<Item>> items;

    void ClearItems();

    void Scroll(WPARAM wParam);
    void ReadAndDisplaySector(word wSector);
};


class DiskImageSectorsDialog::Item {
public:
    word wSector;
    word wAddr;
    word wBegin;
    word wSize;
};
