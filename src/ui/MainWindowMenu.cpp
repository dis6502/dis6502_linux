#include "MainWindowMenu.h"
#include "Menu.h"
#include "Resource.h"
#include "UI.h"
#include "UIApplication.h"
#include <memory>
#include <Windows.h>

extern std::unique_ptr<UIApplication> g_UIApplication;

MainWindowMenu::MainWindowMenu() {
    menu = nullptr;
}

MainWindowMenu::~MainWindowMenu() {
    for (const auto hBitmap : bitmaps) {
        DeleteObject(hBitmap);
    }
}

void MainWindowMenu::CreateControl(HWND hMainWnd) {
    menu = Menu::GetInstance(GetMenu(hMainWnd));

    AddIconToMenu(ID_FILE_NEW, IDI_FILE_NEW);

    AddIconToMenu(ID_FILE_OPEN_CASSETTE_IMAGE_FILE, IDI_FILE_OPEN_CASSETTE_IMAGE_FILE);
    AddIconToMenu(ID_FILE_OPEN_DISK_IMAGE_BOOT_SECTORS, IDI_FILE_OPEN_DISK_IMAGE_BOOT_SECTORS);
    AddIconToMenu(ID_FILE_OPEN_DISK_IMAGE_EXECUTABLE_FILE, IDI_FILE_OPEN_DISK_IMAGE_EXECUTABLE_FILE);
    AddIconToMenu(ID_FILE_OPEN_DISK_IMAGE_SECTORS, IDI_FILE_OPEN_DISK_IMAGE_SECTORS);
    AddIconToMenu(ID_FILE_OPEN_EXECUTABLE_FILE, IDI_FILE_OPEN_EXECUTABLE_FILE);
    AddIconToMenu(ID_FILE_OPEN_RAW_FILE, IDI_FILE_OPEN_RAW_FILE);
    AddIconToMenu(ID_FILE_OPEN_ROM_IMAGE_FILE, IDI_FILE_OPEN_ROM_IMAGE_FILE);

    AddIconToMenu(ID_FILE_ADD_CASSETTE_IMAGE_FILE, IDI_FILE_OPEN_CASSETTE_IMAGE_FILE);
    AddIconToMenu(ID_FILE_ADD_DISK_IMAGE_BOOT_SECTORS, IDI_FILE_OPEN_DISK_IMAGE_BOOT_SECTORS);
    AddIconToMenu(ID_FILE_ADD_DISK_IMAGE_EXECUTABLE_FILE, IDI_FILE_OPEN_DISK_IMAGE_EXECUTABLE_FILE);
    AddIconToMenu(ID_FILE_ADD_DISK_IMAGE_SECTORS, IDI_FILE_OPEN_DISK_IMAGE_SECTORS);
    AddIconToMenu(ID_FILE_ADD_EXECUTABLE_FILE, IDI_FILE_OPEN_EXECUTABLE_FILE);
    AddIconToMenu(ID_FILE_ADD_RAW_FILE, IDI_FILE_OPEN_RAW_FILE);
    AddIconToMenu(ID_FILE_ADD_ROM_IMAGE_FILE, IDI_FILE_OPEN_ROM_IMAGE_FILE);

    AddIconToMenu(ID_FILE_SAVE_DISASSEMBLY_FILES, IDI_FILE_SAVE_DISASSEMBLY_FILES);
    AddIconToMenu(ID_FILE_SAVE_DISK_IMAGE_BOOT_SECTORS, IDI_FILE_SAVE_DISK_IMAGE_BOOT_SECTORS);
    AddIconToMenu(ID_EQUATES_CLEAR_USER_EQUATES, IDI_EQUATES_CLEAR_USER_EQUATES);
    AddIconToMenu(ID_EQUATES_EDIT_USER_EQUATES, IDI_EQUATES_EDIT_USER_EQUATES);
    AddIconToMenu(ID_EQUATES_DEFINE_USER_ADDRESS_RANGE, IDI_EQUATES_DEFINE_USER_ADDRESS_RANGE);
    AddIconToMenu(ID_EQUATES_OPEN_USER_EQUATES, IDI_EQUATES_OPEN_USER_EQUATES);
    AddIconToMenu(ID_EQUATES_SAVE_USER_EQUATES, IDI_EQUATES_SAVE_USER_EQUATES);
    AddIconToMenu(ID_EQUATES_EXPORT_USER_EQUATES, IDI_EQUATES_EXPORT_USER_EQUATES);
    AddIconToMenu(ID_VIEW_PROFILE, IDI_VIEW_PROFILE);
    AddIconToMenu(ID_HELP_ABOUT, IDI_HELP_ABOUT);
}

Menu* MainWindowMenu::GetSubMenuHandle(UINT uMenuPos) const {
    return Menu::GetInstance(GetSubMenu(menu->hMenu, uMenuPos));
}

void MainWindowMenu::SetEnabled(UINT uIDEnableItem, bool enabled) {
    EnableMenuItem(menu->hMenu, uIDEnableItem, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void MainWindowMenu::SetChecked(UINT uIDCheckItem, bool checked) {
    CheckMenuItem(menu->hMenu, uIDCheckItem, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

HBITMAP MainWindowMenu::MakeBitMapTransparent(HBITMAP hbmSrc) {
    HBITMAP hbmNew = NULL_HBITMAP;
    auto hdcSrc = CreateCompatibleDC(NULL_HDC);
    if (hdcSrc != NULL_HDC) {
        auto hdcDst = CreateCompatibleDC(NULL_HDC);
        if (hdcDst != NULL_HDC) {
            BITMAP bm = { 0, 0, 0, 0, 0, 0, nullptr };
            GetObject(hbmSrc, sizeof(bm), &bm);
            SelectObject(hdcSrc, hbmSrc);
            hbmNew = CreateBitmap(bm.bmWidth, bm.bmHeight, bm.bmPlanes, bm.bmBitsPixel, nullptr);
            SelectObject(hdcDst, hbmNew);
            BitBlt(hdcDst, 0, 0, bm.bmWidth, bm.bmHeight, hdcSrc, 0, 0, (SRCCOPY));
            const auto clrTP = GetPixel(hdcDst, 0, 0); // Get color of first pixel at 0,0
            const auto clrBK = GetSysColor(COLOR_MENU); // Get the current background color of the menu

            for (int nRow = 0; nRow < bm.bmHeight; nRow++) { // work our way through all the pixels changing their color
                for (int nCol = 0; nCol < bm.bmWidth; nCol++) { // when we hit our set transparency color.
                    if (GetPixel(hdcDst, nCol, nRow) == clrTP) {
                        SetPixel(hdcDst, nCol, nRow, clrBK);
                    }
                }
            }

            DeleteDC(hdcDst);
        }

        DeleteDC(hdcSrc);
    }

    return hbmNew;
}

void MainWindowMenu::AddIconToMenu(UINT uPosition, int nIconId) {
    ICONINFO iconinfo = { false, 0, 0, NULL_HBITMAP, NULL_HBITMAP };
    auto hIcon = static_cast<HICON>(LoadImage(::g_UIApplication->GetInstanceHandle(), (MAKEINTRESOURCE(nIconId)), IMAGE_ICON, 16, 16, 0));
    GetIconInfo(hIcon, &iconinfo);
    auto hBitmap = iconinfo.hbmColor;
    hBitmap = MakeBitMapTransparent(hBitmap);
    SetMenuItemBitmaps(menu->hMenu, uPosition, MF_BITMAP | MF_BYCOMMAND, hBitmap, hBitmap);
    bitmaps.push_back(hBitmap);
}
