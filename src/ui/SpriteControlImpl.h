/*
** SpriteControlImpl.h
**
** Sprite control implementation used to draw a graphic buffer using a specified Atari graphic mode.
*/
#pragma once

#define NOMINMAX
#include <Windows.h>
#include "Syntax.h"

constexpr UINT WM_SPRITE_BASE = (WM_USER + 1234);

/*
** messages to set or get sprite control parameters
*/
typedef enum {
    WM_SPRITE_SET_BUFFER = WM_SPRITE_BASE,  // wParam = buffer size, lParam = buffer addr
    WM_SPRITE_SET_MODE,                     // wParam = new mode
    WM_SPRITE_GET_MODE,                     // returns current mode
    WM_SPRITE_SET_INDEX,                    // wParam = index into the buffer
    WM_SPRITE_GET_INDEX,                    // returns current index
    WM_SPRITE_SET_NBBYTES,                  // wParam = number of bytes per line to display
    WM_SPRITE_GET_NBBYTES,                  // return current number of bytes per line displayed
    WM_SPRITE_SET_SELECTION,                // wParam = end selection index into the buffer
    WM_SPRITE_GET_SELECTION,                // return current end selection index into the buffer
    WM_SPRITE_GET_INFO,                     // returns information for one of SPRITE_ENUM param
    WM_SPRITE_REFRESH,                      // redraw sprite control
} SPRITE_MSG;

/*
** WM_SPRITE_SET_SELECTION and WM_SPRITE_GET_SELECTION messages can use this
** define for no selection.
*/
#define   SPRITE_NO_SELECTION      0xFFFF

/*
** notifications found in HIWORD(lParam) in WM_COMMAND message
*/
typedef enum {
    SPRITE_INDEX_CHANGED,                   // address of first byte displayed has changed
    SPRITE_NBBYTES_CHANGED,                 // number of bytes per line displayed has changed
    SPRITE_SELECTION_CHANGED                // end of selection has changed
} SPRITE_NOTIFICATION;

/*
** enum for WM_SPRITE_GET_INFO message
*/
typedef enum {
    SPRITE_NBROWS,
    SPRITE_NBLINES,
    SPRITE_NBCOLORS,
    SPRITE_NBBYTESPERLINE,
    SPRITE_NBPIXELSPERBYTE,
    SPRITE_PIXELWIDTH,
    SPRITE_PIXELHEIGHT
} SPRITE_ENUM;

class SpriteControlImpl {
public:
    static constexpr auto CLASS_NAME = L"SpriteControlClass";

    __declspec(dllexport) static void _cdecl RegisterClassFor(HINSTANCE hInstance);

    SpriteControlImpl(HWND hWnd);
    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    HWND hWnd;

    void SetSize(WORD wSize);
    void SetBuffer(WPARAM wParam, LONG_PTR lParam);
    void SetNbBytes(WORD wNbBytes, BOOL bRefresh);
    void SetMode(WORD wMode, BOOL bRefresh);
    void SetIndex(WORD wIndex, BOOL bRefresh);

    void SetSelection(WORD wEnd, BOOL bRefresh);

    LONG GetInfo(WPARAM wParam);

    LONG Create();
    LONG Destroy();

    void PaintAll(HDC hDC);
    LONG Paint();
    void Refresh();

    LONG HScroll(WPARAM wParam, LPARAM lParam);
    LONG VScroll(WPARAM wParam, LPARAM lParam);

    LONG LButtonDown(WPARAM wParam, LPARAM lParam);
    LONG MouseMove(WPARAM wParam, LPARAM lParam);
    LONG LButtonUp(WPARAM wParam, LPARAM lParam);

};
