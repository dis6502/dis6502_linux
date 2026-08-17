/*
** SpriteControlImpl.cpp
**
** Sprite control implementation used to draw a graphic buffer using a specified Atari graphic mode.
*/
#include <algorithm>
#include <memory.h>

#define NOMINMAX
#include <Windows.h>

#include "utils.h"
/* The following types and functions are used
*  DibNew
   DibBitCount
   DibMapToPalette
   DibSetUsage
   DibFree
   DibPtr
   DibWidth
   DibHeight
   DibInfo
*/

#include "SpriteControlImpl.h"

/*
** Structure where buffer size and graphic mode characteristics are saved.
*/
typedef struct SpriteStruct {
    BYTE* pBuf;  // graphic buffer address
    WORD wSize;			// size (in bytes) of the buffer to display
    WORD wIndex;		// index (in bytes) of current position in the buffer
    WORD wMode;			// Antic graphic mode number - 8
    WORD wNbBytes;		// number of bytes per line selected by user
    WORD wEnd;			// last byte of selection (SPRITE_NO_SELECTION = no selection)
} SpriteStruct;

typedef struct LOGPALETTE256 {
    WORD           palVersion;
    WORD           palNumEntries;
    PALETTEENTRY   palPalEntry[256];
} LOGPALETTE256;

/*
** Characteristics of the different Antic graphic modes.
*/
static WORD wSpriteNbRows[] = { 40, 80, 80, 160, 160, 160, 160, 320 };
static WORD wSpriteNbLines[] = { 24, 48, 48, 96, 192, 96, 192, 192 };
static WORD wSpriteNbColors[] = { 4, 2, 4, 2, 2, 4, 4, 1 };
static WORD wSpriteNbBytesPerLine[] = { 10, 10, 20, 20, 20, 40, 40, 40 };
static WORD wSpriteNbPixelsPerByte[] = { 4, 8, 4, 8, 8, 4, 4, 8 };
static WORD wSpritePixelWidth[] = { 8, 4, 4, 2, 2, 2, 2, 1 };
static WORD wSpritePixelHeight[] = { 8, 4, 4, 2, 1, 2, 1, 1 };

/*
** Colors used to draw pixels.
*/
static COLORREF dwSpriteColor[] = { RGB(0, 0, 0), RGB(255, 0, 0), RGB(0, 0, 255), RGB(255, 255, 255) };
static BYTE cSpritePalIndex[] = { 0, 1, 2, 3 };

/*
** bitmap and palette used to draw buffer.
*/
static PDIB lpSpriteDib = 0;
static HPALETTE hSpritePal = 0;
static LOGPALETTE256 sSpritePal;
static UINT uSpriteDIBUsage = DIB_PAL_COLORS;

/*
** macros to get pixel bits from a byte.
*/
#define SPRITE_GET_PIXEL_1(cPixel, bByte) \
     { WORD wPixelNumber; \
     wPixelNumber = 7 - wPixel; \
     cPixel = bByte & cMask8[wPixelNumber]; \
     if (wPixelNumber) \
          cPixel >>= wPixelNumber; \
     cPixel = cSpritePalIndex[cPixel ? 3 : 0]; \
     }

#define SPRITE_GET_PIXEL_2(cPixel, bByte) \
     { WORD wPixelNumber; \
     wPixelNumber = 3 - wPixel; \
     cPixel = bByte & cMask4[wPixelNumber]; \
     if (wPixelNumber) \
          cPixel >>= (wPixelNumber << 1); \
     cPixel = cSpritePalIndex[cPixel]; \
     }


SpriteControlImpl::SpriteControlImpl(HWND hWnd) {
    this->hWnd = hWnd;
}
/*
** set size of buffer to display.
** wSize is the number of bytes of the buffer.
** This function does not redraw control.
*/
void SpriteControlImpl::SetSize(WORD wSize) {
    SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wSize), wSize);
    SetScrollRange(hWnd, SB_VERT, 0, wSize, FALSE);
}

/*
** WM_SPRITE_SET_BUFFER message
** set buffer size and pointer
** This function does not redraw control.
*/
void SpriteControlImpl::SetBuffer(WPARAM wParam, LONG_PTR lParam) {
    SetWindowLongPtr(hWnd, FIELD_OFFSET(SpriteStruct, pBuf), lParam);
    SetSize((WORD)wParam);
}

/*
** WM_SPRITE_SET_NBBYTES message
** set number of bytes per lines.
** if bRefresh is TRUE, control is redrawn.
*/
void SpriteControlImpl::SetNbBytes(WORD wNbBytes, BOOL bRefresh) {
    if (wNbBytes <= 40) {
        SetScrollPos(hWnd, SB_HORZ, wNbBytes, TRUE);
        SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes), wNbBytes);
        if (bRefresh) {
            Refresh();
        }
    }
}

/*
** WM_SPRITE_SET_MODE message
** set graphic mode and its characteristics.
** wMode is an Antic graphic mode (8 to 15).
** if bRefresh is TRUE, control is redrawn.
*/
void SpriteControlImpl::SetMode(WORD wMode, BOOL bRefresh) {

    if ((wMode >= 8) && (wMode <= 15)) {
        wMode -= 8;
        SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode), wMode);
        SetScrollRange(hWnd, SB_HORZ, 1, wSpriteNbBytesPerLine[wMode], FALSE);
        const auto wNbBytes = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes));

        if (wNbBytes > wSpriteNbBytesPerLine[wMode]) {
            SetNbBytes(wSpriteNbBytesPerLine[wMode], FALSE);
            SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), SPRITE_NBBYTES_CHANGED), (LONG_PTR)hWnd);
        }
        else {
            SetNbBytes(wNbBytes, FALSE);
        }

        if (bRefresh) {
            Refresh();
        }
    }
}

/*
** WM_SPRITE_SET_INDEX message
** set first line to display
** if bRefresh is TRUE, control is redrawn.
*/
void SpriteControlImpl::SetIndex(WORD wIndex, BOOL bRefresh) {
    SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wIndex), wIndex);
    SetScrollPos(hWnd, SB_VERT, wIndex, TRUE);

    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_SPRITE_SET_SELECTION message
** set last byte selected.
** if bRefresh is TRUE, control is redrawn.
*/
void SpriteControlImpl::SetSelection(WORD wEnd, BOOL bRefresh) {
    SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd), wEnd);

    if (bRefresh) {
        Refresh();
    }
}

/*
** WM_SPRITE_GET_INFO message
** get information on the wParam subject
*/
LONG SpriteControlImpl::GetInfo(WPARAM wParam) {
    WORD wRet;

    const auto wMode = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));
    switch (wParam) {
    case SPRITE_NBROWS:
        wRet = wSpriteNbRows[wMode];
        break;

    case SPRITE_NBLINES:
        wRet = wSpriteNbLines[wMode];
        break;

    case SPRITE_NBCOLORS:
        wRet = wSpriteNbColors[wMode];
        break;

    case SPRITE_NBBYTESPERLINE:
        wRet = wSpriteNbBytesPerLine[wMode];
        break;

    case SPRITE_NBPIXELSPERBYTE:
        wRet = wSpriteNbPixelsPerByte[wMode];
        break;

    case SPRITE_PIXELWIDTH:
        wRet = wSpritePixelWidth[wMode];
        break;

    case SPRITE_PIXELHEIGHT:
        wRet = wSpritePixelHeight[wMode];
        break;

    default:
        wRet = 0;
        break;
    }

    return (LONG)(DWORD)wRet;
}

/*
** WM_CREATE message
** Initialization of the window private structure in the extra bytes.
** The default setting is no buffer (0 byte LONG) and Antic graphic mode 13.
*/
LONG SpriteControlImpl::Create() {
    HDC hDC;

    /*
    ** prepare bitmap to display buffer
    */
    if ((hDC = GetDC(hWnd)) == 0) {
        return -1;
    }

    if ((lpSpriteDib = DibNew(8, 320, 192))) {
        sSpritePal.palNumEntries = 256;
        sSpritePal.palVersion = 0x300;
        GetSystemPaletteEntries(hDC, 0, 256, sSpritePal.palPalEntry);
        hSpritePal = CreatePalette((LOGPALETTE*)&sSpritePal);

        for (size_t i = 0; i < sizeof(cSpritePalIndex); i++) {
            cSpritePalIndex[i] = GetNearestPaletteIndex(hSpritePal, dwSpriteColor[i]);
        }

        if (DibBitCount(lpSpriteDib) != 4) {
            DibMapToPalette(lpSpriteDib, hSpritePal);
            uSpriteDIBUsage = DIB_PAL_COLORS;
            DibSetUsage(lpSpriteDib, hSpritePal, uSpriteDIBUsage);
        }
        else {
            uSpriteDIBUsage = DIB_RGB_COLORS;
        }
    }
    ReleaseDC(hWnd, hDC);

    /*
    ** set default values
    */
    SetBuffer(0, (LPARAM)0);
    SetMode(13, FALSE);
    SetIndex(0, FALSE);
    SetNbBytes(1, FALSE);
    SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd), SPRITE_NO_SELECTION);

    return (lpSpriteDib ? 0 : -1);
}

LONG SpriteControlImpl::Destroy() {
    if (lpSpriteDib != NULL) {
        DibFree(lpSpriteDib);
    }

    if (hSpritePal) {
        DeleteObject(hSpritePal);
    }

    return NULL;
}

void SpriteControlImpl::PaintAll(HDC hDC) {
    WORD wMode;
    WORD wSize, wIndex;
    WORD wRow, wLine;
    WORD wPixelWidth, wPixelHeight;
    WORD wNbBytes;
    WORD wNbPixelsPerByte;
    WORD wNbBytesPerLine;
    WORD wEnd;
    WORD w, wPixel;
    WORD wMaxRow;
    WORD wRowByte;
    BYTE* lpScreen;
    BYTE cPixel;
    BYTE bByte;
    BYTE cInvert;
    HPALETTE hOldPal;
    static BYTE cMask4[] = { 0x03, 0x0C, 0x30, 0xC0 };
    static BYTE cMask8[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    // get graphic characteristics from window extra bytes.
    wSize = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wSize));
    wNbBytes = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes));
    wIndex = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wIndex));
    auto lpBuffer = (BYTE*)GetWindowLongPtr(hWnd, FIELD_OFFSET(SpriteStruct, pBuf));
    lpBuffer += wIndex;
    wEnd = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd));
    wMode = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));
    wNbBytesPerLine = wSpriteNbBytesPerLine[wMode];
    wNbPixelsPerByte = wSpriteNbPixelsPerByte[wMode];
    wPixelWidth = wSpritePixelWidth[wMode];
    wPixelHeight = wSpritePixelHeight[wMode];

    // draw graphics.
    lpScreen = ((BYTE*)DibPtr(lpSpriteDib)) + ((DWORD)320 * (192 - wPixelHeight));
    wRowByte = wPixelWidth * wNbPixelsPerByte;

    for (wLine = 0; wLine < 192; wLine += wPixelHeight) {
        // stop at the end of memoryInspector buffer
        if (wIndex >= wSize) {
            break;
        }

        // Is this line selected ?
        if ((wEnd != SPRITE_NO_SELECTION) && (wIndex <= wEnd)) {
            cInvert = 0xFF;
        }
        else {
            cInvert = 0;
        }

        // draw the line each byte at a time.
        wMaxRow = std::min<WORD>(wNbBytes, wSize - wIndex);
        for (wRow = 0; wRow < wMaxRow; wRow++) {
            // get byte and invert it if there is a selection
            bByte = (*lpBuffer++) ^ cInvert;

            for (wPixel = 0; wPixel < wNbPixelsPerByte; wPixel++) {
                switch (wMode) {
                case 0:
                    SPRITE_GET_PIXEL_2(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *(lpScreen + 960) = *(lpScreen + 640) = *(lpScreen + 320) =
                            *(lpScreen + 2240) = *(lpScreen + 1920) = *(lpScreen + 1600) =
                            *(lpScreen + 1280) = cPixel;
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 1:
                    SPRITE_GET_PIXEL_1(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *(lpScreen + 960) = *(lpScreen + 640) = *(lpScreen + 320) = cPixel;
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 2:
                    SPRITE_GET_PIXEL_2(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *(lpScreen + 960) = *(lpScreen + 640) = *(lpScreen + 320) = cPixel;
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 3:
                    SPRITE_GET_PIXEL_1(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *(lpScreen + 320) = cPixel;
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 4:
                    SPRITE_GET_PIXEL_1(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 5:
                    SPRITE_GET_PIXEL_2(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *(lpScreen + 320) = cPixel;
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 6:
                    SPRITE_GET_PIXEL_2(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *lpScreen++ = cPixel;
                    }
                    break;

                case 7:
                    SPRITE_GET_PIXEL_1(cPixel, bByte);
                    for (w = 0; w < wPixelWidth; w++) {
                        *lpScreen++ = cPixel;
                    }
                    break;
                }
            }
        }

        // fill the rest of the line (take selection into account)
        if (wRow < wNbBytesPerLine) {
            wRow = (wNbBytesPerLine - wRow) * wRowByte;
            switch (wPixelHeight) {
            case 8:
                memset(lpScreen + 2240, cInvert, wRow);
                memset(lpScreen + 1920, cInvert, wRow);
                memset(lpScreen + 1600, cInvert, wRow);
                memset(lpScreen + 1280, cInvert, wRow);
            case 4:
                memset(lpScreen + 960, cInvert, wRow);
                memset(lpScreen + 640, cInvert, wRow);
            case 2:
                memset(lpScreen + 320, cInvert, wRow);
            case 1:
                memset(lpScreen, cInvert, wRow);
                break;
            }
            lpScreen += wRow;
        }

        wIndex += wMaxRow;
        lpScreen -= 320 * (wPixelHeight + 1);
    }

    // fill the rest of the screen (no selection)
    for (lpScreen = (BYTE*)DibPtr(lpSpriteDib); wLine < 192; wLine++, lpScreen += 320) {
        memset(lpScreen, 0, 320 * sizeof(BYTE));
    }

    // draw bitmap on screen.
    hOldPal = SelectPalette(hDC, hSpritePal, FALSE);
    RealizePalette(hDC);
    StretchDIBits(hDC,
        0, 0, 320, 192,
        0, 0, DibWidth(lpSpriteDib), DibHeight(lpSpriteDib),
        DibPtr(lpSpriteDib), DibInfo(lpSpriteDib),
        uSpriteDIBUsage, SRCCOPY);
    SelectPalette(hDC, hOldPal, FALSE);
}

/*
** WM_PAINT message
** redraw the graphic buffer
*/
LONG SpriteControlImpl::Paint() {
    PAINTSTRUCT ps;
    HDC hDC;

    hDC = BeginPaint(hWnd, &ps);
    PaintAll(hDC);
    EndPaint(hWnd, &ps);

    return NULL;
}

/*
** refresh window
*/
void SpriteControlImpl::Refresh() {
    HDC hDC = GetDC(hWnd);
    if (hDC) {
        PaintAll(hDC);
        ReleaseDC(hWnd, hDC);
    }
}

/*
** WM_HSCROLL message
*/
LONG SpriteControlImpl::HScroll(WPARAM wParam, LPARAM lParam) {
    WORD wOldNbBytes;
    WORD wNbBytes;
    WORD wMaxNbBytes;
    WORD wMode;

    wMode = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));
    wOldNbBytes = wNbBytes = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes));
    wMaxNbBytes = wSpriteNbBytesPerLine[wMode];

    switch (LOWORD(wParam)) {
    case SB_TOP:
        if (wNbBytes > 1) {
            wNbBytes = 1;
        }
        break;

    case SB_BOTTOM:
        if (wNbBytes != wMaxNbBytes) {
            wNbBytes = wMaxNbBytes;
        }
        break;

    case SB_PAGEUP:
        if (wNbBytes > 1) {
            wNbBytes--;
        }
    case SB_LINEUP:
        if (wNbBytes > 1) {
            wNbBytes--;
        }
        break;

    case SB_PAGEDOWN:
        if (wNbBytes != wMaxNbBytes) {
            wNbBytes++;
        }
    case SB_LINEDOWN:
        if (wNbBytes != wMaxNbBytes) {
            wNbBytes++;
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        if ((wNbBytes != HIWORD(wParam)) && (HIWORD(wParam) <= wMaxNbBytes)) {
            wNbBytes = HIWORD(wParam);
        }
        break;
    }
    if (wOldNbBytes != wNbBytes) {
        SetNbBytes(wNbBytes, TRUE);
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), SPRITE_NBBYTES_CHANGED), (LONG_PTR)hWnd);
    }

    return NULL;
}

/*
** WM_VSCROLL message
*/
LONG SpriteControlImpl::VScroll(WPARAM wParam, LPARAM lParam) {
    WORD wOldIndex;
    WORD wIndex;
    WORD wMaxIndex;
    WORD wMode;

    wMode = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));
    wOldIndex = wIndex = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wIndex));
    wMaxIndex = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wSize));

    switch (LOWORD(wParam)) {
    case SB_TOP:
        if (wIndex) {
            wIndex = 0;
        }
        break;

    case SB_BOTTOM:
        if (wIndex != wMaxIndex) {
            wIndex = wMaxIndex;
        }
        break;

    case SB_LINEUP:
        if (wIndex) {
            wIndex--;
        }
        break;

    case SB_LINEDOWN:
        if (wIndex != wMaxIndex) {
            wIndex++;
        }
        break;

    case SB_PAGEUP:
        if (wIndex) {
            wIndex -= std::min(wIndex, wSpriteNbBytesPerLine[wMode]);
        }
        break;

    case SB_PAGEDOWN:
        if (wIndex != wMaxIndex) {
            wIndex += std::min<WORD>(wMaxIndex - wIndex, wSpriteNbBytesPerLine[wMode]);
        }
        break;

    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        if ((wIndex != HIWORD(wParam)) && (HIWORD(wParam) <= wMaxIndex)) {
            wIndex = HIWORD(wParam);
        }
        break;
    }
    if (wOldIndex != wIndex) {
        SetIndex(wIndex, TRUE);
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), SPRITE_INDEX_CHANGED), (LONG_PTR)hWnd);
    }

    return NULL;
}

/*
** WM_LBUTTONDOWN message
*/
LONG SpriteControlImpl::LButtonDown(WPARAM wParam, LPARAM lParam) {

    // get buffer and selection from window extra bytes.
    const auto wOldEnd = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd));
    const auto wNbBytes = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes));
    const auto wIndex = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wIndex));
    const auto wMode = GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));
    const auto wPixelHeight = wSpritePixelHeight[wMode];

    // get buffer offset from selection line.
    const auto wNewEnd = wIndex + ((HIWORD(lParam) / wPixelHeight) * wNbBytes) - 1;

    // if selection has changed, save new selection, redraw control and notify parent window.
    if (wOldEnd != wNewEnd) {
        SetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd), wNewEnd);
        Refresh();
        SendMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), SPRITE_SELECTION_CHANGED), (LONG_PTR)hWnd);
    }

    return NULL;
}

/*
** WM_MOUSEMOVE message
*/
LONG SpriteControlImpl::MouseMove(WPARAM wParam, LPARAM lParam) {
    if (wParam & MK_LBUTTON) {
        return LButtonDown(wParam, lParam);
    }

    return NULL;
}

/*
** WM_LBUTTONUP message
*/
LONG SpriteControlImpl::LButtonUp(WPARAM wParam, LPARAM lParam) {
    return LButtonDown(wParam, lParam);
}


/*
** Sprite window proc.
*/
LRESULT SpriteControlImpl::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SPRITE_SET_BUFFER:
        SetBuffer(wParam, lParam);
        break;

    case WM_SPRITE_SET_MODE:
        SetMode((WORD)wParam, (BOOL)lParam);
        break;

    case WM_SPRITE_SET_INDEX:
        SetIndex((WORD)wParam, (BOOL)lParam);
        break;

    case WM_SPRITE_SET_NBBYTES:
        SetNbBytes((WORD)wParam, (BOOL)lParam);
        break;

    case WM_SPRITE_SET_SELECTION:
        SetSelection((WORD)wParam, (BOOL)lParam);
        break;

    case WM_SPRITE_GET_MODE:
        return GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wMode));

    case WM_SPRITE_GET_INDEX:
        return GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wIndex));

    case WM_SPRITE_GET_NBBYTES:
        return GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wNbBytes));

    case WM_SPRITE_GET_SELECTION:
        return GetWindowWord(hWnd, FIELD_OFFSET(SpriteStruct, wEnd));

    case WM_SPRITE_GET_INFO:
        return GetInfo(wParam);

    case WM_SPRITE_REFRESH:
        Refresh();
        break;

    case WM_CREATE:
        return Create();

    case WM_DESTROY:
        return Destroy();

    case WM_PAINT:
        return Paint();

    case WM_HSCROLL:
        return HScroll(wParam, lParam);

    case WM_VSCROLL:
        return VScroll(wParam, lParam);

    case WM_LBUTTONDOWN:
        return LButtonDown(wParam, lParam);

    case WM_MOUSEMOVE:
        return MouseMove(wParam, lParam);

    case WM_LBUTTONUP:
        return LButtonUp(wParam, lParam);

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return NULL;
}

/*
** Sprite window proc.
*/
LRESULT CALLBACK SpriteWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    SpriteControlImpl control = SpriteControlImpl(hWnd);
    return control.WndProc(message, wParam, lParam);
}

/*
** DLL entry point
*/
BOOL CALLBACK LibMain(HINSTANCE hinst, UINT wDS, UINT cbHeap, DWORD unused) {
    return TRUE;
}

/*
** DLL exit point
*/
INT FAR PASCAL _WEP(INT unused) {
    return TRUE;
}

__declspec(dllexport) void _cdecl  SpriteControlImpl::RegisterClassFor(HINSTANCE hInstance) {

    /*
** Fill in window class structure with parameters that describe the
** sprite control.
*/
    WNDCLASS wc = {
        .style = 0,
        .lpfnWndProc = SpriteWndProc,
        .cbClsExtra = 0,
        .cbWndExtra = sizeof(SpriteStruct),
        .hInstance = hInstance,
        .hIcon = NULL,
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
        .lpszMenuName = NULL,
        .lpszClassName = SpriteControlImpl::CLASS_NAME
    };

    if (!RegisterClass(&wc)) {
        safeExit();
    };
}
